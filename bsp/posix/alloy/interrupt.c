/**
 * Copyright (c) 2021 Wavious LLC.
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <stdint.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

/* System defines. */
#include <errno.h>
#include <pthread.h>
#include <sys/types.h>
#include <sys/time.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include "interrupt.h"

/*******************************************************************************
**                                   MACROS
*******************************************************************************/
// TODO: LConfig pattern
#define MAX_MACHINE_INTERRUPTS  (32)
#define IRQ_THREAD_PORT         (6000)

/*******************************************************************************
**                            FUNCTION DECLARATIONS
*******************************************************************************/
static void * __irq_worker_thread( void *pvParameters );
static void __attribute__((constructor))  irq_subsystem_init(void);

/*******************************************************************************
**                          STRUCTURE DECLARATIONS
*******************************************************************************/
struct irq_message
{
    uint16_t irq_num;   // < represents the IRQ that should be raised.
    uint32_t is_set;    // < represents if the IRQ is set or not.
} __attribute__((packed));

static struct
{
    alloy_interrupt_handler_t handler;
    void *prv_args;
    bool enabled;
    bool pending;
} irq_table[MAX_MACHINE_INTERRUPTS];

/*******************************************************************************
**                              IMPLEMENTATIONS
*******************************************************************************/
int alloy_interrupt_set_pending(int irq, bool pending)
{
    if (irq >= MAX_MACHINE_INTERRUPTS)
    {
        return -1;
    }

    irq_table[irq].pending = pending;
    return 0;
}

static void alloy_interrupt_invoke(int irq)
{
    if (irq_table[irq].handler != NULL &&
        irq_table[irq].enabled)
    {
        irq_table[irq].handler(irq, irq_table[irq].prv_args);
    }
}

void alloy_interrupt_tick(void)
{
    // Invoke pending interrupts in order of priority
    for (int i = MAX_MACHINE_INTERRUPTS; i > 0; i--)
    {
        if (irq_table[i - 1].pending)
        {
            alloy_interrupt_invoke(i);
        }
    }
}

int alloy_interrupt_register_handler(int irq,
                                     alloy_interrupt_handler_t handler,
                                     void *args)
{
    if (irq >= MAX_MACHINE_INTERRUPTS)
    {
        return -1;
    }

    irq_table[irq].handler = handler;
    irq_table[irq].prv_args = args;
    return 0;
}

int alloy_interrupt_enable(int irq)
{
    if (irq >= MAX_MACHINE_INTERRUPTS)
    {
        return -1;
    }

    irq_table[irq].enabled = true;
    return 0;
}

int alloy_interrupt_disable(int irq)
{
    if (irq >= MAX_MACHINE_INTERRUPTS)
    {
        return -1;
    }

    irq_table[irq].enabled = false;
    return 0;
}

static void __attribute__((constructor)) irq_subsystem_init(void)
{
    // Create a worker thread to monitor for socket activity
    pthread_t thread_id;

    // Start a thread to run xIRQTask
    int err = pthread_create(&thread_id, NULL, &__irq_worker_thread, NULL);
    if (err)
    {
        printf("irq_subsystem_init: Error couldn't create __irq_worker_thread\n");
    }
}

static void * __irq_worker_thread( void *params )
{
    fd_set master, read_fs;
    int fd_new, recv_len, listener, fd_max;
    struct sockaddr_in addr = {0};
    struct sockaddr_in client_addr = {0};
    struct irq_message message;
    struct timeval tv = {.tv_sec = 0, .tv_usec = 500000}; // 500 ms

    printf("__irq_worker_thread started...\n");

    FD_ZERO(&master);
    FD_ZERO(&read_fs);

    // Create a Socket to listen for incoming messages
    if ((listener = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) == -1)
    {
        printf("__irq_worker_thread: Couldn't open listener socket.\n");
        exit(1);
    }

    int yes = 1;
    if (setsockopt(listener, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)) == -1)
    {
        printf("__irq_worker_thread: socket option failed.\n");
        exit(1);
    }

    // Open socket on port at localhost address for listening
    addr.sin_family = AF_INET;
    addr.sin_port = htons(IRQ_THREAD_PORT);
    addr.sin_addr.s_addr = inet_addr("127.0.0.1");

    if ((bind(listener, (struct sockaddr *)&addr, sizeof(struct sockaddr))) == -1)
    {
        printf("__irq_worker_thread: Couldn't bind socket.\n");
        exit(1);
    }

    if (listen(listener, 1) == -1)
    {
        printf("__irq_worker_thread: listen failed.\n");
        exit(1);
    }

    // add listener to the master set of descriptors
    FD_SET(listener, &master);

    fd_max = listener; // only descriptor so its the max

    // Event Loop
    while (1)
    {
        // Sleep the task
        read_fs = master;

        // Timeout or something to read
        select(fd_max + 1, &read_fs, NULL, NULL, &tv);

        // See if descriptor is ready
        for (uint8_t i = 0; i <= fd_max; i++)
        {
            if (FD_ISSET(i, &read_fs))
            {
                // New connection incoming
                if (i == listener)
                {
                    socklen_t addr_len = sizeof(struct sockaddr_in);
                    fd_new = accept(listener, (struct sockaddr *) &client_addr, &addr_len);
                    if (fd_new == -1)
                    {
                        if (errno == EINTR)
                        {
                            continue;
                        }
                        printf("__irq_worker_thread: accept failed: %d\n", errno);
                        exit(1);
                    }

                    // Add new descriptor to the list
                    FD_SET(fd_new, &master);
                    if (fd_new > fd_max)
                    {
                        fd_max = fd_new;
                    }
                    printf("__irq_worker_thread: New connection from %s\n", inet_ntoa(client_addr.sin_addr));
                }
                else
                {
                    // Read data from the client
                    recv_len = recv(i, &message, sizeof(struct irq_message), MSG_WAITALL);

                    // Connection closed
                    if (recv_len == 0)
                    {
                        printf("__irq_worker_thread: connection lost\n");
                        close(i);
                        FD_CLR(i, &master);
                    }

                    // Set IRQ state
                    alloy_interrupt_set_pending(message.irq_num, message.is_set);

                }   // if (i == listener)
            } // if (FD_ISSET(i, &read_fs))
        } // for loop

        // Invoke any pending interrupts
        alloy_interrupt_tick();
    };
}
