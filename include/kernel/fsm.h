/**
 * Copyright (c) 2021 Wavious LLC.
 *
 * SPDX-License-Identifier: Apache-2.0
 */
#ifndef _KERNEL_FSM_H_
#define _KERNEL_FSM_H_

/* Standard includes. */
#include <stdint.h>
#include <stdbool.h>

/* FreeRTOS includes. */
#include <FreeRTOS.h>

// Forward Declaration
struct fsm_t;

/** @brief State Handler Function Definition */
typedef void (*state_handler_t)(struct fsm_t* fsm, void* data);

/** @brief Guard Handler Function Definition */
typedef bool (*guard_handler_t)(struct fsm_t *fsm, void *data);

/** @brief Exit Handler Function Definition */
typedef void (*exit_handler_t)(struct fsm_t* fsm);

/** @brief  State Change CB Function Defition */
typedef void (*state_change_cb_t)(struct fsm_t *fsm, uint8_t state, void *args);

/**
 * @brief   State Function Structure
 *
 * @details Structure for storing all functions associated with a given
 *          state in the FSM.
 *
 * state    State function executed in the state.
 * guard    Guard function executed prior to state handler.
 * exit     Exit function executed when leaving the state.
 */
typedef struct state_func_t
{
    state_handler_t state;
    guard_handler_t guard;
    exit_handler_t  exit;
} state_func_t;

/**
 * @brief   Finite State Machine Structure
 *
 * @details Finite State Machine structure used for tracking
 *          current state machine state.
 *
 * instance         Pointer to specific FSM instance
 *                  (typically specific FSM structure).
 * current_state    Current state of the FSM.
 * next_state       Next state of the FSM.
 * event_handled    Flag to track if event has been handled.
 * data             Function state specific data.
 * state_table      Function pointer table for state functions for all states.
 * state_cb         Function pointer to callback when state transition occurs
 *                  (optionally non-null).
 */
typedef struct fsm_t
{
                void    *instance;
                void    *data;
                const state_func_t *state_table;
                state_change_cb_t state_cb;
                void *cb_args;
    volatile    uint8_t current_state;
                uint8_t next_state;
                bool    event_handled;
} fsm_t;

/**
 * @brief   Finite State Machine Task Initialization
 *
 * @details Initializes FSM Task for handling FSM events.
 *
 * @param[in]   uxPriority      FSM Task priority.
 * @param[in]   usStackDepth    Stack depth in words for the FSM Task.
 * @param[in]   uxQueueLength   Length of FSM Event Queue.
 *
 * @return  Returns whether initialization was successful.
 * @retval  pdTRUE if successful.
 * @retval  pdFALSE otherwise.
 */
BaseType_t xFSMTaskInit(UBaseType_t uxPriority,
                        configSTACK_DEPTH_TYPE usStackDepth,
                        UBaseType_t uxQueueLength);

/**
 * @brief   Finite State Machine Initialization
 *
 * @details Initializes Finite State Machine.
 *
 * @param[in]   fsm             pointer to FSM instance.
 * @param[in]   instance        pointer to specific FSM structure instance.
 * @param[in]   state_table     array of function pointers for all stats of the FSM.
 *
 * @return      void.
 */
void fsm_init(fsm_t *fsm, void *instance, const state_func_t *state_table);

/**
 * @brief   Finite State Machine Register State Change Callback
 *
 * @details Registers a callback that will be invoked whenever the FSM changes
 *          states.
 *
 * @param[in]   fsm     pointer to FSM instance.
 * @param[in]   cb      pointer to State Change Callback implementation.
 * @param[in]   args    pointer to State Change Callback arguments.
 *
 * @return      void.
 */
void fsm_register_state_change_callback(fsm_t *fsm, const state_change_cb_t cb, void *args);

/**
 * @brief   Finite State Machine Handle External Event
 *
 * @details Common external event handler for all FSMs. Should be called by any
 *          external event function that a FSM implements. Responsible for
 *          "kicking" the state machine.
 *
 * @param[in]   fsm         pointer to FSM instance.
 * @param[in]   new_state   new state that FSM should move to.
 * @param[in]   data        pointer to function specific data.
 *
 * @return      void.
 */
void fsm_handle_external_event(fsm_t *fsm, uint8_t new_state, void *data);

/**
 * @brief   Finite State Machine Handle Interrupt Event
 *
 * @details Common interrupt event handler for all FSMs. Should be called by any
 *          ISR function that a FSM implements. Responsible for "kicking" the
 *          state machine.
 *
 * @param[in]   fsm         pointer to FSM instance.
 * @param[in]   new_state   new state that FSM should move to.
 * @param[in]   data        pointer to function specific data.
 *
 * @return      void.
 */
void fsm_handle_interrupt_event(fsm_t *fsm, uint8_t new_state, void *data);

/**
 * @brief   Finite State Machine Handle Internal Event
 *
 * @details Common internal event handler for all FSMs. Should be called by any
 *          internal event function that a FSM implements. Typically used in
 *          state handler functions.
 *
 * @param[in]   fsm         pointer to FSM instance.
 * @param[in]   new_state   new state that FSM should move to.
 * @param[in]   data        pointer to function specific data.
 *
 * @return      void.
 */
void fsm_handle_internal_event(fsm_t *fsm, uint8_t new_state, void *data);

#endif /* _KERNEL_FSM_H_ */
