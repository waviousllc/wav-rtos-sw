/**
 * Copyright (c) 2021 Wavious LLC.
 *
 * SPDX-License-Identifier: Apache-2.0
 */

/* Standard includes. */
#include <stddef.h>
#include <kernel/fsm.h>

/* FreeRTOS includes. */
#include <FreeRTOS.h>
#include <task.h>
#include <queue.h>

/*******************************************************************************
**                            STRUCTURE DECLARATIONS
*******************************************************************************/
/**
 * @brief   FSM Event Enumerations
 *
 * @details Enumerations for types of FSM Transitions.
 *
 * eFSMTransitionInternalEvent    internal state transition.
 * eFSMTransitionExternalEvent    external state transition.
 */
typedef enum
{
    eFSMTransitionInternalEvent,
    eFSMTransitionExternalEvent,
} eFSMTransistion_t;

/**
 * @brief   FSM Task Event Structure
 *
 * @details Structure for transition events submitted to FSM task.
 *
 * eTransitionType      the type of transition.
 * new_state            the new state transitioning to.
 * fsm                  pointer to fsm that should transition.
 * data                 pointer to event specific data.
 */
typedef struct
{
    eFSMTransistion_t eTransitionType;
    uint8_t new_state;
    fsm_t *fsm;
    void *data;
} FSMTaskEvent_t;

/*******************************************************************************
**                           VARIABLE DECLARATIONS
*******************************************************************************/
static TaskHandle_t xFSMTaskHandle = NULL;
static QueueHandle_t xFSMEventQueue = NULL;

/*******************************************************************************
**                            FUNCTION DECLARATIONS
*******************************************************************************/
/**
 * @brief   FSM Task
 *
 * @details FSM Task that updates FSM state as it executes.
 *          This is shared for all FSM instances.
 *
 * @param[in]   pvParameters    unused.
 *
 * @return      void.
 */
static void prvFSMTask(void *pvParameters);

/**
 * @brief   Finite State Machine Update
 *
 * @details Updates FSM to handled any outstanding events.
 *          Should only be called by when external event occurs.
 *
 * @param fsm   pointer to fsm instance
 *
 * @return      void.
 */
static void fsm_update(fsm_t *fsm);

/**
 * @brief   Private Handle Internal Event
 *
 * @details Internal function for handling an internal FSM event.
 *
 * @param[in]   fsm         pointer to FSM instance.
 * @param[in]   new_state   new state that FSM should move to.
 * @param[in]   data        pointer to function specific data.
 *
 * @return      void.
 */
static void private_handle_internal_event(fsm_t *fsm, uint8_t new_state, void *data);

/**
 * @brief   Private Handle External Event
 *
 * @details Internal function for handling an external FSM event.
 *
 * @param[in]   fsm         pointer to FSM instance.
 * @param[in]   new_state   new state that FSM should move to.
 * @param[in]   data        pointer to function specific data.
 *
 * @return      void.
 */
static void private_handle_external_event(fsm_t *fsm, uint8_t new_state, void *data);

/*******************************************************************************
**                              IMPLEMENTATIONS
*******************************************************************************/
BaseType_t xFSMTaskInit(UBaseType_t uxPriority,
                        configSTACK_DEPTH_TYPE usStackDepth,
                        UBaseType_t uxQueueLength)
{
    configASSERT(xFSMEventQueue == NULL);
    configASSERT(xFSMTaskHandle == NULL);

    // Create Queue if not already done
    if (xFSMEventQueue != NULL || xFSMTaskHandle != NULL)
    {
        return pdFALSE;
    }
    // Create Event Queue
    xFSMEventQueue = xQueueCreate(uxQueueLength, sizeof(FSMTaskEvent_t));
    configASSERT(xFSMEventQueue != NULL);
    if (xFSMEventQueue == NULL)
    {
        return pdFALSE;
    }

    // Create Task if not already done
    if (xTaskCreate(prvFSMTask,
                    "FSM Task",
                    usStackDepth,
                    NULL,
                    uxPriority,
                    &xFSMTaskHandle) == pdFALSE)
    {
        return pdFALSE;
    }
    return pdTRUE;
}

void fsm_init(fsm_t *fsm, void *instance, const state_func_t *state_table)
{
    fsm->instance = instance;
    fsm->current_state = 0;
    fsm->state_table = state_table;
    fsm->event_handled = 1;
    fsm->data = NULL;
    fsm->state_cb = NULL;
    fsm->cb_args = NULL;
}

void fsm_register_state_change_callback(fsm_t *fsm, const state_change_cb_t cb, void *args)
{
    fsm->state_cb = cb;
    fsm->cb_args = args;
}

void fsm_handle_internal_event(fsm_t *fsm, uint8_t new_state, void *data)
{
    configASSERT(xTaskGetCurrentTaskHandle() == xFSMTaskHandle);
    private_handle_internal_event(fsm, new_state, data);
}

void fsm_handle_external_event(fsm_t *fsm, uint8_t new_state, void *data)
{
    FSMTaskEvent_t xEvent;
    // Don't submit into queue if already running as FSM Task
    if (xTaskGetCurrentTaskHandle() == xFSMTaskHandle)
    {
        private_handle_external_event(fsm, new_state, data);
        return;
    }
    xEvent.eTransitionType = eFSMTransitionExternalEvent;
    xEvent.fsm = fsm;
    xEvent.new_state = new_state;
    xEvent.data = data;
    xQueueSendToBack(xFSMEventQueue, &xEvent, portMAX_DELAY);
}

void fsm_handle_interrupt_event(fsm_t *fsm, uint8_t new_state, void *data)
{
    BaseType_t xHigherPriorityTaskWoken = pdFALSE;
    FSMTaskEvent_t xEvent;
    xEvent.eTransitionType = eFSMTransitionExternalEvent;
    xEvent.fsm = fsm;
    xEvent.new_state = new_state;
    xEvent.data = data;
    xQueueSendToBackFromISR(xFSMEventQueue, &xEvent, &xHigherPriorityTaskWoken);
    portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
}

static void fsm_update(fsm_t *fsm)
{
    bool guard_result = true;

    while (!fsm->event_handled)
    {
        state_handler_t handler = fsm->state_table[fsm->next_state].state;
        guard_handler_t guard = fsm->state_table[fsm->next_state].guard;
        exit_handler_t  exit = fsm->state_table[fsm->current_state].exit;

        fsm->event_handled = 1;

        if (guard != NULL)
        {
            guard_result = guard(fsm, fsm->data);
        }

        if (guard_result)
        {
            if (fsm->next_state != fsm->current_state)
            {
                if (exit != NULL)
                {
                    exit(fsm);
                }
            }
            fsm->current_state = fsm->next_state;
            if (fsm->state_cb)
            {
                fsm->state_cb(fsm, fsm->current_state, fsm->cb_args);
            }
            handler(fsm, fsm->data);
        }
    }
}

static void private_handle_internal_event(fsm_t *fsm, uint8_t new_state, void *data)
{
    fsm->data = data;
    fsm->next_state = new_state;
    fsm->event_handled = 0;
}

static void private_handle_external_event(fsm_t *fsm, uint8_t new_state, void *data)
{
    private_handle_internal_event(fsm, new_state, data);
    fsm_update(fsm);
}

static void prvFSMTask(void *pvParameters)
{
    FSMTaskEvent_t xEvent;

    for(;;)
    {
        // Block indefinitely
        xQueueReceive(xFSMEventQueue, &xEvent, portMAX_DELAY);

        switch(xEvent.eTransitionType)
        {
            case eFSMTransitionExternalEvent:
                private_handle_external_event(xEvent.fsm, xEvent.new_state, xEvent.data);
                break;
            case eFSMTransitionInternalEvent:
                private_handle_internal_event(xEvent.fsm, xEvent.new_state, xEvent.data);
                break;
        }
    }
}
