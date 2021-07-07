/**
 * Copyright (c) 2021 Wavious LLC.
 *
 * SPDX-License-Identifier: Apache-2.0 
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
**                           VARIABLE DECLARATIONS
*******************************************************************************/
static TaskHandle_t xNotificationTaskHandle = NULL;
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
BaseType_t xNotificationTaskInit(UBaseType_t uxPriority,
                                 configSTACK_DEPTH_TYPE usStackDepth)
{
    configASSERT(xNotificationTaskHandle == NULL);

    // Can only call once
    if (xNotificationTaskHandle != NULL)
    {
        return pdFALSE;
    }

    // Create Task
    if (xTaskCreate(prvNotificationTask,
                    "Notification Task",
                    usStackDepth,
                    NULL,
                    uxPriority,
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
    if (xNotificationTaskHandle == NULL)
    {
        return;
    }

    xTaskNotify(xNotificationTaskHandle, xNotification, eSetValueWithOverwrite);
}

static void prvNotificationTask(void *pvParameters)
{
    Notification_t xNotification;
    ListItem_t *pxItem;
    NotificationEndpoint_t *pxEndpoint;

    for(;;)
    {
        // Block indefinitely
        xTaskNotifyWait(0, 0, &xNotification, portMAX_DELAY);

        listFOR_EACH_LIST_ITEM(pxItem, &xEndpointList)
        {
            pxEndpoint = (NotificationEndpoint_t *) listGET_LIST_ITEM_OWNER(pxItem);

            // Notify
            pxEndpoint->cb.xHandler(xNotification, pxEndpoint->cb.pvArgs);
        }
    }
}
