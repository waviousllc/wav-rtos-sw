/**
 * Copyright (c) 2021 Wavious LLC.
 *
 * SPDX-License-Identifier: Apache-2.0
 */

/* Standard includes. */
#include <stdint.h>
#include <stddef.h>
#include <string.h>

/* Socket APIs. */
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/ip.h>
#include <netinet/in.h>
#include <arpa/inet.h>

/* RTOS includes. */
#include <FreeRTOS.h>
#include <task.h>

/* Kernel includes. */
#include <kernel/io.h>
#include <kernel/irq.h>
#include <kernel/completion.h>
#include <kernel/messenger.h>

/* Driver includes. */
#include <messenger/driver.h>
#include <messenger/messages_general.h>

/*******************************************************************************
**                                   MACROS
*******************************************************************************/
#define TASK_PRI    (2)

#define GET_SOCK(messenger)             ((int) messenger->irq.ack)
#define SET_SOCK(messenger, sock)       (messenger->irq.ack =  (uint16_t) sock)
#define SET_SEND_PORT(messenger, port)  (messenger->irq.req = (uint16_t) port)
#define GET_SEND_PORT(messenger)        (messenger->irq.req)

#ifndef CONFIG_MESSENGER_PORT_START
    #define CONFIG_MESSENGER_PORT_START (5000)
#endif /* CONFIG_MESSENGER_PORT_START */

/*******************************************************************************
**                            FUNCTION DECLARATIONS
*******************************************************************************/
static void xServerTask( void *pvParameters );

// Initial port value
static int next_port = CONFIG_MESSENGER_PORT_START;

/*******************************************************************************
**                              IMPLEMENTATIONS
*******************************************************************************/

void messenger_init_reg_if(message_interface_t *messenger, UBaseType_t base, uint16_t req_irq, uint16_t ack_irq)
{
    // Create a socket
    struct sockaddr_in addr = {0};
    int ret;

    // Create a Socket to listen for incoming messages
    int sock_fd = socket(AF_INET, SOCK_DGRAM , IPPROTO_UDP);

    // ensure descriptor was given
    configASSERT(sock_fd >= 0);

    // Open socket on port at localhost address for listening
    addr.sin_family = AF_INET;
    addr.sin_port = htons(next_port++);
    addr.sin_addr.s_addr = inet_addr("127.0.0.1");

    SET_SEND_PORT(messenger, next_port++);

    // bind socket to port
    ret = bind(sock_fd, (struct sockaddr *) &addr, sizeof(struct sockaddr_in));
    configASSERT(ret >= 0);

    messenger->address = base;
    SET_SOCK(messenger, sock_fd);
    vInitCompletion(&messenger->send_complete);

    // Create a worker thread to monitor for socket activity
    xTaskCreate( xServerTask, "Messenger Socket Worker", configMINIMAL_STACK_SIZE, messenger, TASK_PRI, NULL );
}

bool messenger_send_reg_if(void *dev, void *buff, size_t len)
{
    message_interface_t *messenger = (message_interface_t *) dev;
    struct sockaddr_in addr = {0};
    int sock_fd = GET_SOCK(messenger);

    // Open socket on port at localhost address for listening
    addr.sin_family = AF_INET;
    addr.sin_port = htons(GET_SEND_PORT(messenger));
    addr.sin_addr.s_addr = inet_addr("127.0.0.1");

    // Send message
    size_t send_len;
    taskENTER_CRITICAL();
    {
        send_len = sendto(sock_fd, buff, len, 0, (struct sockaddr*) &addr, sizeof(struct sockaddr_in));
    }
    taskEXIT_CRITICAL();
    return send_len == len;
}

bool messenger_receive_reg_if(void *dev, void *buff, size_t len)
{
    message_interface_t *messenger = (message_interface_t *) dev;
    int sock_fd = GET_SOCK(messenger);
    size_t recv_len;
    recv_len = recvfrom(sock_fd, buff, len, 0, NULL, NULL);
    vComplete(&messenger->send_complete);
    return recv_len == len;
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

static void xServerTask( void *pvParameters )
{
    BaseType_t xHigherPriorityTaskWoken = pdFALSE;
    int recv_len;
    message_interface_t *messenger = (message_interface_t *) pvParameters;
    int sock_fd = GET_SOCK(messenger);
    uint8_t buff[sizeof(Message_t)];

    // Receive forever
    while(1)
    {
        recv_len = recvfrom(sock_fd, buff, sizeof(Message_t), MSG_PEEK | MSG_WAITALL, NULL, NULL);

        // This is precautionary; Rather ignore than fail for stability
        if (recv_len != sizeof(Message_t))
        {
            continue;
        }

        vReInitCompletion(&messenger->send_complete);

        // Notify thread that message is available
        vSubmitReceiveEventFromISR(messenger->address, &xHigherPriorityTaskWoken);

        // Block until message is read in the other thread
        vWaitForCompletion(&messenger->send_complete);
    }
}
