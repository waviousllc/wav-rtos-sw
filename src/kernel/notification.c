/**
 * Copyright (c) 2021 Wavious LLC.
 *
 * SPDX-License-Identifier: GPL-3.0
 */
/* Standard includes. */
#include <stddef.h>

/* FreeRTOS includes. */
#include <FreeRTOS.h>
#include <task.h>
#include <queue.h>

/* Kernel includes. */
#include <kernel/list.h>
#include <kernel/notification.h>

/*******************************************************************************
**                                   MACROS
*******************************************************************************/
#ifndef CONFIG_NOTIFICATION_QUEUE_LENGTH
    #define CONFIG_NOTIFICATION_QUEUE_LENGTH    (8)
#endif /* CONFIG_NOTIFICATION_QUEUE_LENGTH */
#ifndef CONFIG_NOTIFICATION_TASK_PRIORITY
    #define CONFIG_NOTIFICATION_TASK_PRIORITY   (configMAX_PRIORITIES - 2)
#endif /* CONFIG_NOTIFICATION_TASK_PRIORITY */

#define CONFIG_NOTIFICATION_STACK_SIZE_WORDS    (configMINIMAL_STACK_SIZE)

/*******************************************************************************
**                           VARIABLE DECLARATIONS
*******************************************************************************/
static TaskHandle_t xNotificationTaskHandle = NULL;
static QueueHandle_t xQueue = NULL;
static List_t xEndpointList;

/*******************************************************************************
**                            FUNCTION DECLARATIONS
*******************************************************************************/
/**
 * @brief   Notification Task
 *
 * @details Notification Task that notifies endpoints of specific notifications
 *          when they are generated.
 *
 * @param[in]   pvParameters    unused.
 *
 * @return      void.
 */
static void prvNotificationTask(void *pvParameters);

/*******************************************************************************
**                              IMPLEMENTATIONS
*******************************************************************************/
BaseType_t xNotificationTaskInit(void)
{
    configASSERT(xQueue == NULL);
    configASSERT(xNotificationTaskHandle == NULL);

    // Can only call once
    if (xQueue != NULL || xNotificationTaskHandle != NULL)
    {
        return pdFALSE;
    }

    // Create Queue
    xQueue = xQueueCreate(CONFIG_NOTIFICATION_QUEUE_LENGTH, sizeof(Notification_t));
    configASSERT(xQueue != NULL);
    if (xQueue == NULL)
    {
        return pdFALSE;
    }

    // Create Task
    if (xTaskCreate(prvNotificationTask,
                    "Notification Task",
                    CONFIG_NOTIFICATION_STACK_SIZE_WORDS,
                    NULL,
                    CONFIG_NOTIFICATION_TASK_PRIORITY,
                    &xNotificationTaskHandle) == pdFALSE)
    {
        return pdFALSE;
    }

    // Initialize Endpoint List
    vListInitialise(&xEndpointList);

    return pdTRUE;
}

void vInitNotificationEndpoint(NotificationEndpoint_t *pxEndpoint,
                               NotificationHandler_t xHandler,
                               void *pvArgs)
{
    vListInitialiseItem(&pxEndpoint->xItem);
    listSET_LIST_ITEM_OWNER(&pxEndpoint->xItem, pxEndpoint);
    pxEndpoint->cb.xHandler = xHandler;
    pxEndpoint->cb.pvArgs = pvArgs;
}

void vRegisterNotificationEndpoint(NotificationEndpoint_t *pxEndpoint)
{
    if (!listLIST_IS_INITIALISED(&xEndpointList))
    {
        return;
    }

    vListInsertEnd(&xEndpointList, &pxEndpoint->xItem);
}

void vSendNotification(Notification_t xNotification)
{
    if (xQueue == NULL)
    {
        return;
    }

    xQueueSendToBack(xQueue, &xNotification, portMAX_DELAY);
}

static void prvNotificationTask(void *pvParameters)
{
    Notification_t xNotification;
    ListItem_t *pxItem;
    NotificationEndpoint_t *pxEndpoint;

    for(;;)
    {
        // Block indefinitely
        xQueueReceive(xQueue, &xNotification, portMAX_DELAY);

        listFOR_EACH_LIST_ITEM(pxItem, &xEndpointList)
        {
            pxEndpoint = (NotificationEndpoint_t *) listGET_LIST_ITEM_OWNER(pxItem);

            // Notify
            pxEndpoint->cb.xHandler(xNotification, pxEndpoint->cb.pvArgs);
        }
    }
}
