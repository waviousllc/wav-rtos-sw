/**
 * Copyright (c) 2021 Wavious LLC.
 *
 * SPDX-License-Identifier: Apache-2.0
 */

/* Standard includes. */
#include <stdint.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <unistd.h>

/* System defines. */
#include <errno.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/time.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

/* FreeRTOS includes. */
#include <FreeRTOS.h>
#include <task.h>
#include <semphr.h>

/* Alloy defines. */
#include <alloy/interrupt.h>

/*******************************************************************************
**                                   MACROS
*******************************************************************************/
#define ALLOY_PORT         (6000)

/*******************************************************************************
**                          STRUCTURE DECLARATIONS
*******************************************************************************/
enum
{
    ALLOY_MSG_TYPE_IRQ,
    ALLOY_MSG_TYPE_REG_WRITE,
    ALLOY_MSG_TYPE_REG_READ,
};

struct irq_message
{
    uint16_t irq_num;   // < represents the IRQ that should be raised.
    uint8_t is_set;     // < represents if the IRQ is set or not.
}__attribute__((packed));

struct reg_message
{
    uint64_t addr;      // < represents teh address that was rad
    uint32_t data;      // < represents the data at the address
}__attribute__((packed));

union alloy_msg_data
{
    struct reg_message reg;
    struct irq_message irq;
};

struct alloy_msg_hdr
{
    uint8_t type;
    uint8_t len;
};

struct alloy_msg
{
    struct alloy_msg_hdr hdr;
    union alloy_msg_data data;
} __attribute__((packed));

/*******************************************************************************
**                            FUNCTION DECLARATIONS
*******************************************************************************/
static void __attribute__((constructor)) alloy_core_init(void);
static bool prv_socket_send_safe(int socket, void *buff, size_t len);
static size_t prv_socket_recv_safe(int socket, void *buff, size_t len);
static void prv_handle_irq_msg(struct alloy_msg *msg);
static void prv_handle_reg_msg(struct alloy_msg *msg);
static void xAlloySocketWorker(void *args);
static void xAlloyInterruptContext(void *args);

/*******************************************************************************
**                          Variable DECLARATIONS
*******************************************************************************/
static int socket_fd;
static TaskHandle_t xRegReadTask = NULL;
static SemaphoreHandle_t mutex;
static SemaphoreHandle_t sem;

/*******************************************************************************
**                              IMPLEMENTATIONS
*******************************************************************************/
uint32_t alloy_reg_read(uint64_t addr)
{
    uint32_t reg_data;
    struct alloy_msg message;
    memset(&message, 0, sizeof(struct alloy_msg));
    message.hdr.type = ALLOY_MSG_TYPE_REG_READ;
    message.hdr.len = sizeof(struct reg_message);
    message.data.reg.addr = addr;
    message.data.reg.data = 0x0;

    // Send read request
    prv_socket_send_safe(socket_fd, &message, sizeof(struct alloy_msg));

    xRegReadTask = xTaskGetCurrentTaskHandle();
    xTaskNotifyWait(0, 0, &reg_data, portMAX_DELAY);

    // Return register value
    return reg_data;
}

void alloy_reg_write(uint64_t addr, uint32_t data)
{
    struct alloy_msg message;
    memset(&message, 0, sizeof(struct alloy_msg));
    message.hdr.type = ALLOY_MSG_TYPE_REG_WRITE;
    message.hdr.len = sizeof(struct reg_message);
    message.data.reg.addr = addr;
    message.data.reg.data = data;

    // Send write request
    prv_socket_send_safe(socket_fd, &message, sizeof(struct alloy_msg));
}

static void xAlloyInterruptContext(void *args)
{
    while (1)
    {
        xSemaphoreTake(sem, portMAX_DELAY);
        alloy_interrupt_tick();
    }
}

static void __attribute__((constructor)) alloy_core_init(void)
{
    // Initialize the interrupts
    alloy_interrupt_init();

    // Create a sempahore
    sem = xSemaphoreCreateBinary();
    mutex = xSemaphoreCreateMutex();

    // Interrupt context
    xTaskCreate( &xAlloyInterruptContext,
                 "Alloy IRQ Context",
                 configMINIMAL_STACK_SIZE,
                 NULL,
                 configMAX_PRIORITIES - 1,
                 NULL );

    // Socket worker thread
    xTaskCreate( &xAlloySocketWorker,
                "Alloy Socket Worker",
                configMINIMAL_STACK_SIZE,
                NULL,
                configMAX_PRIORITIES - 1,
                NULL );
}

static void xAlloySocketWorker(void *args)
{
    int recv_len;
    struct sockaddr_in server_addr = {0};
    struct alloy_msg message;
    const TickType_t xDelay = pdMS_TO_TICKS(100);

    // Create a Socket to listen for incoming messages
    if ((socket_fd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
    {
        printf("alloy_socket_thread: Couldn't open socket.\n");
        exit(1);
    }

    // Connect to server
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(ALLOY_PORT);
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    bzero(&(server_addr.sin_zero), sizeof(server_addr.sin_zero));

    if ((connect(socket_fd, (struct sockaddr *)&server_addr, sizeof(struct sockaddr))) == -1)
    {
        printf("alloy_socket_thread: Couldn't connect to server %d.\n", errno);
        exit(1);
    }

    // Make non-blocking so that thread can sleep via FreeRTOS
    fcntl(socket_fd, F_SETFL, O_NONBLOCK);

    printf("alloy_socket_thread: connected to server\n");
    // Event Loop
    while (1)
    {
        vTaskDelay(xDelay);

        // Read hdr from the client
        recv_len = prv_socket_recv_safe(socket_fd, &message.hdr, sizeof(struct alloy_msg_hdr));

        if (recv_len == 0 || message.hdr.len == 0)
        {
            continue;
        }

        // Receive rest of the message
        recv_len = prv_socket_recv_safe(socket_fd, &message.data, message.hdr.len);

        // Fatal error
        if (recv_len == 0)
        {
            printf("Warning: Received incomplete Alloy command\n");
            continue;
        }

        switch(message.hdr.type)
        {
            case ALLOY_MSG_TYPE_IRQ:
                prv_handle_irq_msg(&message);
                break;
            case ALLOY_MSG_TYPE_REG_READ:
                prv_handle_reg_msg(&message);
                break;
            default:
                break;
        }

        // Signal to IRQ thread to invoke any pending interrupts
        xSemaphoreGive(sem);
    }
}

static bool prv_socket_send_safe(int socket, void *buff, size_t len)
{
    if (socket == -1)
    {
        return false;
    }

    xSemaphoreTake(mutex, portMAX_DELAY);
    send(socket, buff, len, 0);
    xSemaphoreGive(mutex);
    return true;
}

static size_t prv_socket_recv_safe(int socket, void *buff, size_t len)
{
    if (socket == -1)
    {
        return -1;
    }

    xSemaphoreTake(mutex, portMAX_DELAY);
    int recv_len = recv(socket, buff, len, MSG_WAITALL);
    xSemaphoreGive(mutex);

    if (recv_len == -1)
    {
        printf("something went wrong reading from client.\n");
        exit(1);
    }

    // Client closed connection
    if (recv_len == 0)
    {
        close(socket_fd);
        printf("Remote endpoint closed connection\n");
        exit(1);
    }

    return recv_len;
}

static void prv_handle_irq_msg(struct alloy_msg *msg)
{
    alloy_interrupt_set_pending(msg->data.irq.irq_num, msg->data.irq.is_set);
}

static void prv_handle_reg_msg(struct alloy_msg *msg)
{
    configASSERT(xRegReadTask != NULL);
    xTaskNotify(xRegReadTask, msg->data.reg.data, eSetValueWithOverwrite);
    xRegReadTask = NULL;
}
