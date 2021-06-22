/**
 * Copyright (c) 2021 Wavious LLC.
 *
 * SPDX-License-Identifier: Apache-2.0
 */
#ifndef _KERNEL_MESSENGER_H_
#define _KERNEL_MESSENGER_H_

/* Standard includes. */
#include <stdint.h>
#include <stdbool.h>

/* Kernel includes. */
#include <kernel/list.h>
#include <kernel/completion.h>

/**
 * @brief   Message Data Structure
 *
 * @details Structure for messages sent between Wavious chiplets.
 *
 * id       Message ID.
 * data     Message Data.
 */
typedef struct Message_t
{
    uint32_t id;
    uint32_t data;
} Message_t;

/** @brief  Physical Interface Send Function */
typedef bool (*PhyInterfaceSendFunction_t)(void *dev, void *buff, size_t len);

/** @brief  Physical Interface Receive Function */
typedef bool (*PhyInterfaceReceiveFunction_t)(void *dev, void *buff, size_t len);

/** @brief  Physical Interface Validation Function */
typedef bool (*PhyInterfaceValidateFunction_t)(void *dev, void *buff, size_t len);

/**
 * @brief   Messenger Interface Structure
 *
 * @details Container for storing available driver interface in a FreeRTOS
 *          list.
 *
 * xRecvEvent       Internal completion event used to indicate when message
 *                  was received.
 * xInterfaceItem   List item that is used to store in list of interfaces.
 * xMessageList     List of messages that have been received on the interface.
 * intf.dev         Raw driver device structure. Defined by driver layer.
 * intf.xSend       Function pointer to device driver send function.
 * intf.xRecv       Function pointer to device driver receive function.
 * intf.xValidate   Function pointer to device driver validation function.
 */
typedef struct MesageInterface_t
{
    Completion_t                        xRecvEvent;
    ListItem_t                          xInterfaceItem;
    List_t                              xMessageList;
    struct
    {
        void                            *dev;
        PhyInterfaceSendFunction_t      xSend;
        PhyInterfaceReceiveFunction_t   xRecv;
        PhyInterfaceValidateFunction_t  xValidate;
    } intf;
} MessageInterface_t;

/**
 * @brief   Messenger Task Initialization
 *
 * @details Initializes the Messenger Task. This layer provides common interface
 *          for receiving messages from any driver implementation.
 *
 * @param[in]   uxPriority      Messenger Task priority.
 * @param[in]   usStackDepth    Stack depth in words for the Messenger Task.
 * @param[in]   uxQueueLength   Length of Messenger Event Queue.
 *
 * @return      returns whether initialization was successful.
 *
 * @retval      pdTRUE if successful.
 * @retval      pdFALSE otherwise.
 */
BaseType_t xMessengerTaskInit(UBaseType_t uxPriority,
                              configSTACK_DEPTH_TYPE usStackDepth,
                              UBaseType_t uxQueueLength);

/**
 * @brief   Messenger Interface Register Physical Interface
 *
 * @details Registers a physical interface with the Messenger Task. This
 *          physical interface will be associated with messages sent to a
 *          particular address. At least one interface must be register prior
 *          to using the Messenger Task.
 *
 * @param[in]   dev         generic pointer to raw device driver to register.
 * @param[in]   uAddress    address associated with the device.
 * @param[in]   xSend       function pointer to send implementation for the
 *                          device.
 * @param[in]   xRecv       function pointer to receive implementation for the
 *                          device.
 * @param[in]   xValidate   function pointer to validate implementation for the
 *                          device.
 *
 *
 * @return      returns whether interface was registered successfully.
 * @retval      pdTRUE if successful.
 * @retval      pdFALSE otherwise.
 */
BaseType_t xMessengerInterfaceRegisterPhyInterface(void *dev,
                                                   UBaseType_t uAddress,
                                                   PhyInterfaceSendFunction_t xSend,
                                                   PhyInterfaceSendFunction_t xRecv,
                                                   PhyInterfaceValidateFunction_t xValidate);

/**
 * @brief   Submit Receive Event From ISR
 *
 * @details Function that raw driver should call when a message is received.
 *          The Messenger Task will request the data from the driver when
 *          it is ready to process the received message. In other words, it
 *          implements the bottom half of the receive ISR.
 *
 * @param[in]   uAddress                    Address of interface that received
 *                                          message.
 * @param[out]  pxHigherPriorityTaskWoken   Flag to indicate if event caused
 *                                          higher priority task to wake.
 *
 * @return      void.
 */
void vSubmitReceiveEventFromISR(UBaseType_t uAddress,
                                BaseType_t *pxHigherPriorityTaskWoken);

/**
 * @brief   Send Message
 *
 * @details Function for sending a message. This should be called from higher
 *          layers into the Messenger Task, which will forward to the matching
 *          interface.
 *
 * @param[in]   uAddress    Address of interface to send message with.
 * @param[in]   pxMessage   Pointer to message to send.
 *
 * @return      returns whether message was sent successfully.
 * @retval      pdTRUE if sent.
 * @retval      pdFALSE otherwise.
 */
BaseType_t xSendMessage(UBaseType_t uAddress, Message_t *pxMessage);

/**
 * @brief   Receive Message
 *
 * @details Function for receiving a message. This should be called from higher
 *          layers into the Messenger Task. The function will block until there
 *          is a message to be received.
 *
 * @param[in]   uAddress    Address of interface to receive message from.
 * @param[out]  pxMessage   Pointer to message to receive.
 *
 * @return      returns whether message was received successfully.
 * @retval      pdTRUE if received.
 * @retval      pdFALSE if no matching interface.
 */
BaseType_t xReceiveMessage(UBaseType_t uAddress, Message_t *pxMessage);

#endif /* _KERNEL_MESSENGER_H_ */
