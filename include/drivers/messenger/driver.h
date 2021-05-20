/**
 * Copyright (c) 2021 Wavious LLC.
 *
 * SPDX-License-Identifier: Apache-2.0
 */
#ifndef _MESSENGER_REG_IF_H_
#define _MESSENGER_REG_IF_H_

/* Standard includes. */
#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

/* FreeRTOs includes. */
#include <FreeRTOS.h>

/* Kernel includes. */
#include <kernel/completion.h>

/**
 * @brief   Messenger Driver Interface Structure
 *
 * @details Stores all necessary information for communicating with a messenger
 *          hardware interface.
 *
 * send_complete    used to coordinate message send tasks.
 * address          base address of messenger interface.
 * irq.req          RX request input irq number.
 * irq.ack          TX acknowledge input irq number.
 */
typedef struct message_interface_t
{
    Completion_t send_complete;
    UBaseType_t  address;
    struct
    {
        uint16_t req;
        uint16_t ack;
    } irq;
} message_interface_t;

/**
 * @brief   Messenger Initialization
 *
 * @details Initialization function for message interface.
 *          NOTE: It is expected a device function to register messenger
 *          with the Messenger Interface using the function
 *          xMessengerInterfaceRegisterPhyInterface prior to calling
 *          messenger_init on messenger.
 *
 * @param[in]   messenger   pointer to messenger.
 * @param[in]   base        base address of the messenger device.
 * @param[in]   req_irq     irq number for request interrupt.
 * @param[in]   ack_irq     irq number for acknowledgement interrupt.
 *
 * @return      void.
 */
void messenger_init_reg_if(message_interface_t *messenger, UBaseType_t base, uint16_t req_irq, uint16_t ack_irq);

/**
 * @brief   Messenger Send Register Interface
 *
 * @details Register Interface function for sending a message to the MCU.
 *
 * @param[in]   dev         generic pointer to messenger device.
 * @param[in]   buff        pointer to data to send.
 * @param[in]   len         amount of data to send.
 *
 * @return      returns if message was sent successfully.
 *
 * @retval      true if sent successfully.
 * @retval      false otherwise.
 */
bool messenger_send_reg_if(void *dev, void *buff, size_t len);

/**
 * @brief   Messenger Receive Register Interface
 *
 * @details Register Interface function for receiving a message from the MCU.
 *
 * @param[in]   dev         generic pointer to messenger device.
 * @param[out]  buff        pointer to where to store data.
 * @param[in]   len         amount of data to send.
 *
 * @return      returns if message was recieved successfully.
 *
 * @retval      true if read successfully.
 * @retval      false otherwise.
 */
bool messenger_receive_reg_if(void *dev, void *buff, size_t len);

/**
 * @brief   Messenger Validate Register Interface
 *
 * @details Register Interface function for validating a message from the MCU.
 *
 * @param[in]   dev         generic pointer to messenger device.
 * @param[out]  buff        pointer to where to store data.
 * @param[in]   len         amount of data to send.
 *
 * @return      returns if message is valid.
 *
 * @retval      true if message is valid.
 * @retval      false otherwise.
 */
bool messenger_validate_reg_if(void *dev, void *buff, size_t len);

#endif /* _MESSENGER_REG_IF_H_ */
