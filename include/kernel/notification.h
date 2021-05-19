/**
 * Copyright (c) 2021 Wavious LLC.
 *
 * SPDX-License-Identifier: GPL-3.0
 */
#ifndef _KERNEL_NOTIFICATION_H_
#define _KERNEL_NOTIFICATION_H_

/* Standard includes. */
#include <stdint.h>
#include <stdbool.h>

/* Kernel includes. */
#include <kernel/list.h>

/** @brief  Notification Type
 *
 * @details Represents a specific notification value.
 */
typedef BaseType_t Notification_t;

/** @brief  Notification Handler */
typedef void (*NotificationHandler_t)(Notification_t notification, void *args);

/**
 * @brief   Notification Endpoint Structure
 *
 * @details Notification Endpoint that processes all notifications that
 *          are created. Endpoints filter for notifications they are interested
 *          in rather than being called per specific notifications. Endpoints
 *          are called from the NotificationTask context.
 *
 * xItem        private structure used to add to endpoint list.
 * cb.xHandler  callback handler function to call when notification is received.
 * cb.pvArgs    private callback data pased back to handler.
 */
typedef struct NotificationEndpoint_t
{
    ListItem_t xItem;
    struct
    {
        NotificationHandler_t xHandler;
        void *pvArgs;
    } cb;
} NotificationEndpoint_t;

/**
 * @brief   Notification Task Initialization
 *
 * @details Initializes Notification Task for handling notifications.
 *
 * @return  returns whether initialization was successful.
 * @retval  pdTRUE if successful.
 * @retval  pdFALSE otherwise.
 */
BaseType_t xNotificationTaskInit(void);

/**
 * @brief   Initialize Notification Endpoint
 *
 * @details Initializes a Notification Endpoint. Must be called prior to
 *          registering the endpoint.
 *
 * @param[in]   pxEndpoint  pointer to the Notification Endpoint to initialize.
 * @param[in]   xHandler    pointer to the handler callback.
 * @param[in]   pvArgs      pointer to the handler callback arguments.
 *
 * @return      void
 */
void vInitNotificationEndpoint(NotificationEndpoint_t *pxEndpoint,
                               NotificationHandler_t xHandler,
                               void *pvArgs);

/**
 * @brief   Register Notification Endpoint
 *
 * @details Registers a Notification Endpoint with the Notification Task.
 *          Endpoints will be called via their handler function when ANY
 *          notification is received. Endpoints are called in order that
 *          they are registered.
 *
 * @param[in]   pxEndpoint  pointer to the Notification Endpoint to register.
 *
 * @return      void
 */
void vRegisterNotificationEndpoint(NotificationEndpoint_t *pxEndpoint);

/**
 * @brief   Send Notification To Endpoint
 *
 * @details Sends a notification to all endpoints.
 *
 * @param[in]   xNotification   Notification value to send.
 *
 * @return      void
 */
void vSendNotification(Notification_t xNotification);

#endif /* _KERNEL_NOTIFICATION_H_ */
