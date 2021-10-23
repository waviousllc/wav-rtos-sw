/**
 * Copyright (c) 2021 Wavious LLC.
 *
 * SPDX-License-Identifier: Apache-2.0
 */

/* Standard includes. */
#include <stdint.h>
#include <stddef.h>
#include <string.h>

/* Kernel includes. */
#include <kernel/io.h>
#include <kernel/irq.h>
#include <kernel/completion.h>
#include <kernel/messenger.h>

/* Driver includes. */
#include <messenger/driver.h>
#include <messenger/messages_general.h>
#include <board/board.h>

/*******************************************************************************
**                                   MACROS
*******************************************************************************/
#if WAV_MESSENGER_INTF_HOST == 1
    #define MSGINTF_TX_ID__ADR      WAV_MCUINTF_HOST2MCU_MSG_ID__ADR
    #define MSGINTF_TX_DATA__ADR    WAV_MCUINTF_HOST2MCU_MSG_DATA__ADR
    #define MSGINTF_TX_REQ__ADR     WAV_MCUINTF_HOST2MCU_MSG_REQ__ADR
    #define MSGINTF_TX_ACK__ADR     WAV_MCUINTF_HOST2MCU_MSG_ACK__ADR
    #define MSGINTF_RX_ID__ADR      WAV_MCUINTF_MCU2HOST_MSG_ID__ADR
    #define MSGINTF_RX_DATA__ADR    WAV_MCUINTF_MCU2HOST_MSG_DATA__ADR
    #define MSGINTF_RX_REQ__ADR     WAV_MCUINTF_MCU2HOST_MSG_REQ__ADR
    #define MSGINTF_RX_ACK__ADR     WAV_MCUINTF_MCU2HOST_MSG_ACK__ADR
#else
    #define MSGINTF_TX_ID__ADR      WAV_MCUINTF_MCU2HOST_MSG_ID__ADR
    #define MSGINTF_TX_DATA__ADR    WAV_MCUINTF_MCU2HOST_MSG_DATA__ADR
    #define MSGINTF_TX_REQ__ADR     WAV_MCUINTF_MCU2HOST_MSG_REQ__ADR
    #define MSGINTF_TX_ACK__ADR     WAV_MCUINTF_MCU2HOST_MSG_ACK__ADR
    #define MSGINTF_RX_ID__ADR      WAV_MCUINTF_HOST2MCU_MSG_ID__ADR
    #define MSGINTF_RX_DATA__ADR    WAV_MCUINTF_HOST2MCU_MSG_DATA__ADR
    #define MSGINTF_RX_REQ__ADR     WAV_MCUINTF_HOST2MCU_MSG_REQ__ADR
    #define MSGINTF_RX_ACK__ADR     WAV_MCUINTF_HOST2MCU_MSG_ACK__ADR
#endif /* WAV_MESSENGER_INTF_HOST == 1 */

/*******************************************************************************
**                            FUNCTION DECLARATIONS
*******************************************************************************/
/**
 * @brief   Messenger Clear Acknowledgement Register Interface
 *
 * @details Register Interface function for clearing the acknowledgement
 *          register that is associated with acknowledgements from the MCU.
 *
 * @param[in]   messenger   pointer to messenger.
 *
 * @return      void
 */
static void messenger_clear_ack_reg_if(message_interface_t *messenger);

/**
 * @brief   Messenger Clear Request Register Interface
 *
 * @details Register Interface function for clearing the request
 *          register that is associated with requests from the MCU.
 *
 * @param[in]   messenger   pointer to messenger.
 *
 * @return      void
 */
static void messenger_clear_req_reg_if(message_interface_t *messenger);

/**
 * @brief   Messenger Acknowledgement IRQ Handler
 *
 * @details Interrupt service routine (ISR) called when interface completes
 *          transimission of a message. Completion is marked by receiving an
 *          acknowledgement from the other endpoint. This ISR will unblock the
 *          sending task and allow it to proceed.
 *
 * @param[in]   irq_num     the IRQ number that fired.
 * @param[in]   args        generic pointer to args registered with handler.
 *
 * @return      void
 */
static void messenger_ack_irq_handler(int irq_num, void *args);

/**
 * @brief   Messenger Request IRQ Handler
 *
 * @details Interrupt service routine (ISR) called when an interface receives a
 *          message.
 *
 * @param[in]   irq_num     the IRQ number that fired.
 * @param[in]   args        generic pointer to args registered with handler.
 *
 * @return      void
 */
static void messenger_req_irq_handler(int irq_num, void *args);

/*******************************************************************************
**                              IMPLEMENTATIONS
*******************************************************************************/

void messenger_init_reg_if(message_interface_t *messenger, UBaseType_t base, uint16_t req_irq, uint16_t ack_irq)
{
    messenger->address = base;
    messenger->irq.ack = ack_irq;
    messenger->irq.req = req_irq;
    vInitCompletion(&messenger->send_complete);

    // Register IRQ Handlers
    request_irq(messenger->irq.ack, messenger_ack_irq_handler, messenger);
    request_irq(messenger->irq.req, messenger_req_irq_handler, messenger);

    disable_irq(messenger->irq.ack);
}

bool messenger_send_reg_if(void *dev, void *buff, size_t len)
{
    message_interface_t *messenger = (message_interface_t *) dev;
    Message_t *msg = (Message_t *) buff;

    if (messenger == NULL)
    {
        return false;
    }

    if (len != sizeof(Message_t))
    {
        return false;
    }

    vReInitCompletion(&messenger->send_complete);

    // Send Message
    reg_write(messenger->address + MSGINTF_TX_ID__ADR, msg->id);
    reg_write(messenger->address + MSGINTF_TX_DATA__ADR, msg->data);

    // Enable ACK IRQ
    enable_irq(messenger->irq.ack);

    // Send Request
    reg_write(messenger->address + MSGINTF_TX_REQ__ADR, 0x1);

    // Wait for ack
    vWaitForCompletion(&messenger->send_complete);

    return true;
}

bool messenger_receive_reg_if(void *dev, void *buff, size_t len)
{
    message_interface_t *messenger = (message_interface_t *) dev;
    Message_t *msg = (Message_t *) buff;

    if (messenger == NULL)
    {
        return false;
    }

    if (len != sizeof(Message_t))
    {
        return false;
    }

    // Read Message Registers
    msg->id = reg_read(messenger->address + MSGINTF_RX_ID__ADR);
    msg->data = reg_read(messenger->address + MSGINTF_RX_DATA__ADR);

    // Send back an ACK
    reg_write(messenger->address + MSGINTF_RX_ACK__ADR, 0x1);

    return true;
}

bool messenger_validate_reg_if(void *dev, void *buff, size_t len)
{
    Message_t *msg = (Message_t *) buff;

    switch(msg->id)
    {
        case MESSAGE_GENERAL_INTF_READY_RESP:
        case MESSAGE_GENERAL_FW_VER_REQ:
        case MESSAGE_GENERAL_FW_VER_RESP:
        case MESSAGE_GENERAL_HW_VER_REQ:
        case MESSAGE_GENERAL_HW_VER_RESP:
        case MESSAGE_GENERAL_MCU_BOOT_REQ:
        case MESSAGE_GENERAL_MCU_BOOT_RESP:
            return true;
        default:
            break;
    }

    return false;
}

static void messenger_clear_ack_reg_if(message_interface_t *messenger)
{
    reg_write(messenger->address + MSGINTF_TX_ACK__ADR, 0x1);
}

static void messenger_clear_req_reg_if(message_interface_t *messenger)
{
    reg_write(messenger->address + MSGINTF_RX_REQ__ADR, 0x1);
}

/*-----------------------------------------------------------*/
static void messenger_ack_irq_handler(int irq_num, void *args)
{
    BaseType_t xHigherPriorityTaskWoken = pdFALSE;
    (void) irq_num;
    message_interface_t *messenger = (message_interface_t *) args;

    // Make sure this is the correct inerface
    configASSERT(irq_num == messenger->irq.ack);

    disable_irq(messenger->irq.ack);

    // Clear IRQ
    messenger_clear_ack_reg_if(messenger);

    // Mark send as complete
    vCompleteFromISR(&messenger->send_complete, &xHigherPriorityTaskWoken);

    // Return to highest priority task
    portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
}

/*-----------------------------------------------------------*/
static void messenger_req_irq_handler(int irq_num, void *args)
{
    BaseType_t xHigherPriorityTaskWoken = pdFALSE;
    (void) irq_num;
    message_interface_t *messenger = (message_interface_t *) args;

    // Make sure this is the correct inerface
    configASSERT(irq_num == messenger->irq.req);

    // Clear IRQ
    messenger_clear_req_reg_if(messenger);

    // Pass up messenger stack
    vSubmitReceiveEventFromISR(messenger->address, &xHigherPriorityTaskWoken);

    // Return to highest priority task
    portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
}
