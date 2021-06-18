/**
 * Copyright (c) 2021 Wavious LLC.
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <FreeRTOS.h>
#include <task.h>
#include <semphr.h>
#include <queue.h>

#include <kernel/messenger.h>
#include <kernel/list.h>
#include <kernel/notification.h>

/*******************************************************************************
**                                   MACROS
*******************************************************************************/
#ifndef CONFIG_MESSAGE_QUEUE_LENGTH
    #define CONFIG_MESSAGE_QUEUE_LENGTH         (4)
#endif /* CONFIG_NOTIFICATION_QUEUE_LENGTH */
#ifndef CONFIG_MESSENGER_TASK_PRIORITY
    #define CONFIG_MESSENGER_TASK_PRIORITY      (configMAX_PRIORITIES - 4)
#endif /* CONFIG_NOTIFICATION_TASK_PRIORITY */

#define CONFIG_EVENT_QUEUE_LENGTH               (CONFIG_MESSAGE_QUEUE_LENGTH >> 1)
#define CONFIG_MESSENGER_STACK_SIZE_WORDS       (configMINIMAL_STACK_SIZE)

#define FREE_DESCRIPTOR(DESCRIPTOR)                                         \
    {                                                                       \
        uxListRemove(&DESCRIPTOR->xMessageItem);                            \
        vListInsertEnd(&xFreeDescriptorsList, &DESCRIPTOR->xMessageItem);   \
        xSemaphoreGive(xMessageDescriptorsSemaphore);                       \
    }

/*******************************************************************************
**                           VARIABLE DECLARATIONS
*******************************************************************************/
static TaskHandle_t xMessengerTaskHandle = NULL;
static QueueHandle_t xEventQueue = NULL;
static List_t xInterfaceList;
static List_t xFreeDescriptorsList;
static SemaphoreHandle_t xMessageDescriptorsSemaphore = NULL;

/* Pool of Message Descriptors that exist in the system. */
static struct MessageDescriptor_t
{
    Message_t msg;
    ListItem_t xMessageItem;
} xMessageDescriptors[CONFIG_MESSAGE_QUEUE_LENGTH];

/*******************************************************************************
**                            FUNCTION DECLARATIONS
*******************************************************************************/
/**
 * @brief   Messenger Task
 *
 * @details Messenger Task that handles receiving messages from registered
 *          interfaces.
 *
 * @param[in]   pvParameters    unused.
 *
 * @return      void.
 */
static void prvMessengerTask(void *pvParameters);

/**
 * @brief   Messenger Interface Lookup Physical Interface
 *
 * @details Finds the physical interface associated with the address, if it
 *          exists.
 *
 * @param[in]   uAddress    physical address associated with the interface.
 *
 * @return      returns pointer to physical interface.
 * @retval      pointer to physical interface if found.
 * @retval      NULL otherwise.
 */
MessageInterface_t *pxLookupPhyInterface(UBaseType_t uAddress);

/*******************************************************************************
**                              IMPLEMENTATIONS
*******************************************************************************/
BaseType_t xMessengerTaskInit(void)
{
    configASSERT(xEventQueue == NULL);
    configASSERT(xMessengerTaskHandle == NULL);

    // Can only call once
    if (xEventQueue != NULL ||
        xMessengerTaskHandle != NULL ||
        xMessageDescriptorsSemaphore != NULL)
    {
        return pdFALSE;
    }

    // Create Event Queue
    xEventQueue = xQueueCreate(CONFIG_EVENT_QUEUE_LENGTH, sizeof(UBaseType_t));
    configASSERT(xEventQueue != NULL);
    if (xEventQueue == NULL)
    {
        return pdFALSE;
    }

    // Create Descriptors Semaphore
    xMessageDescriptorsSemaphore = xSemaphoreCreateCounting((UBaseType_t) CONFIG_MESSAGE_QUEUE_LENGTH,
                                                            (UBaseType_t) CONFIG_MESSAGE_QUEUE_LENGTH);
    configASSERT(xMessageDescriptorsSemaphore != NULL);
    if (xMessageDescriptorsSemaphore == NULL)
    {
        return pdFALSE;
    }

    // Create Task
    if (xTaskCreate(prvMessengerTask,
                    "Messenger Task",
                    CONFIG_MESSENGER_STACK_SIZE_WORDS,
                    NULL,
                    CONFIG_MESSENGER_TASK_PRIORITY,
                    &xMessengerTaskHandle) == pdFALSE)
    {
        return pdFALSE;
    }

    // Initialize Endpoint List
    vListInitialise(&xInterfaceList);

    // Initialize Free Descriptor List
    vListInitialise(&xFreeDescriptorsList);

    // Initialize Descriptors
    for (uint8_t i = 0; i < CONFIG_MESSAGE_QUEUE_LENGTH; i++)
    {
        vListInitialiseItem(&xMessageDescriptors[i].xMessageItem);
        listSET_LIST_ITEM_OWNER(&xMessageDescriptors[i].xMessageItem, &xMessageDescriptors[i]);

        // Add to free list
        vListInsertEnd(&xFreeDescriptorsList, &xMessageDescriptors[i].xMessageItem);
    }
    return pdTRUE;
}

BaseType_t xMessengerInterfaceRegisterPhyInterface(void *dev,
                                                   UBaseType_t uAddress,
                                                   PhyInterfaceSendFunction_t xSend,
                                                   PhyInterfaceSendFunction_t xRecv,
                                                   PhyInterfaceValidateFunction_t xValidate)
{
    MessageInterface_t *pxInterface;

    pxInterface = pvPortMalloc(sizeof(MessageInterface_t));
    if (pxInterface == NULL)
    {
        return pdFALSE;
    }

    // Initialize Interface List
    if (!listLIST_IS_INITIALISED(&xInterfaceList))
    {
        return pdFALSE;
    }

    // Address is taken
    if (pxLookupPhyInterface(uAddress) != NULL)
    {
        return pdFALSE;
    }

    // Initialize interface item
    pxInterface->intf.dev = dev;
    pxInterface->intf.xSend = xSend;
    pxInterface->intf.xRecv = xRecv;
    pxInterface->intf.xValidate = xValidate;

    // Init completion
    vInitCompletion(&pxInterface->xRecvEvent);

    // Initialize Message List
    vListInitialise(&pxInterface->xMessageList);

    // Intialize xInterfaceItem
    vListInitialiseItem(&pxInterface->xInterfaceItem);
    listSET_LIST_ITEM_VALUE(&pxInterface->xInterfaceItem, uAddress);
    listSET_LIST_ITEM_OWNER(&pxInterface->xInterfaceItem, pxInterface);
    vListInsert(&xInterfaceList, &pxInterface->xInterfaceItem);
    return pdTRUE;
}

void vSubmitReceiveEventFromISR(UBaseType_t uAddress,
                                BaseType_t *pxHigherPriorityTaskWoken)
{
    if (xEventQueue == NULL)
    {
        return;
    }

    xQueueSendToBackFromISR(xEventQueue,
                            &uAddress,
                            pxHigherPriorityTaskWoken);
}

BaseType_t xSendMessage(UBaseType_t uAddress, Message_t *pxMessage)
{
    MessageInterface_t *pxInterface = pxLookupPhyInterface(uAddress);
    if (pxInterface == NULL)
    {
        return pdFALSE;
    }

    // Validate Message for interface
    if (!pxInterface->intf.xValidate(pxInterface->intf.dev, (void *) pxMessage, sizeof(Message_t)))
    {
        return pdFALSE;
    }

    // Send
    return pxInterface->intf.xSend(pxInterface->intf.dev, (void *) pxMessage, sizeof(Message_t));
}

BaseType_t xReceiveMessage(UBaseType_t uAddress, Message_t *pxMessage)
{
    MessageInterface_t *pxInterface = pxLookupPhyInterface(uAddress);
    struct MessageDescriptor_t *pxDescriptor;
    ListItem_t *pxMessageItem;
    if (pxInterface == NULL)
    {
        return pdFALSE;
    }

    // Wait until message has been received
    vWaitForCompletion(&pxInterface->xRecvEvent);

    if (listLIST_IS_EMPTY(&pxInterface->xMessageList))
    {
        return pdFALSE;
    }

    // Remove from the list
    pxMessageItem = listGET_HEAD_ENTRY(&pxInterface->xMessageList);
    pxDescriptor = (struct MessageDescriptor_t *) listGET_LIST_ITEM_OWNER(pxMessageItem);
    pxMessage->id = pxDescriptor->msg.id;
    pxMessage->data = pxDescriptor->msg.data;
    FREE_DESCRIPTOR(pxDescriptor);

    return pdTRUE;
}

MessageInterface_t *pxLookupPhyInterface(UBaseType_t uAddress)
{
    ListItem_t *pxItem;
    listFOR_EACH_LIST_ITEM(pxItem, &xInterfaceList)
    {
        if (listGET_LIST_ITEM_VALUE(pxItem) == uAddress)
        {
            return (MessageInterface_t *)listGET_LIST_ITEM_OWNER(pxItem);
        }
    }
    return NULL;
}

static void prvMessengerTask(void *pvParameters)
{
    UBaseType_t uAddress;
    MessageInterface_t *pxInterface;
    Message_t msg;
    struct MessageDescriptor_t *pxDescriptor;
    for(;;)
    {
        // Block indefinitely
        xQueueReceive(xEventQueue, &uAddress, portMAX_DELAY);

        pxInterface = pxLookupPhyInterface(uAddress);
        // Drop
        if (pxInterface == NULL)
        {
            continue;
        }

        // Drop
        if (!pxInterface->intf.xRecv(pxInterface->intf.dev, &msg, sizeof(Message_t)))
        {
            continue;
        }

        // Drop
        if (!pxInterface->intf.xValidate(pxInterface->intf.dev, &msg, sizeof(Message_t)))
        {
            continue;
        }

        // Block until free descriptor available
        xSemaphoreTake(xMessageDescriptorsSemaphore, portMAX_DELAY);

        // Critical Section since modifying free list
        taskENTER_CRITICAL();
        {
            pxDescriptor = (struct MessageDescriptor_t *) listGET_OWNER_OF_HEAD_ENTRY(&xFreeDescriptorsList);
            uxListRemove(&pxDescriptor->xMessageItem);
        }
        taskEXIT_CRITICAL();

        // Add to interface message list
        pxDescriptor->msg.id = msg.id;
        pxDescriptor->msg.data = msg.data;
        vListInsertEnd(&pxInterface->xMessageList, &pxDescriptor->xMessageItem);

        vComplete(&pxInterface->xRecvEvent);
    }
}
