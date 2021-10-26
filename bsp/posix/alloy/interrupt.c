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
#define MAX_MACHINE_INTERRUPTS  (32)

/*******************************************************************************
**                          STRUCTURE DECLARATIONS
*******************************************************************************/
static struct
{
    alloy_interrupt_handler_t handler;
    void *prv_args;
    bool enabled;
} irq_table[MAX_MACHINE_INTERRUPTS];

static uint32_t pending_mask = 0x0;
static uint32_t _prev_pending = 0x0;

/*******************************************************************************
**                              IMPLEMENTATIONS
*******************************************************************************/
void alloy_interrupt_init(void)
{
    for (uint8_t i = 0; i < MAX_MACHINE_INTERRUPTS; i++)
    {
       irq_table[i].enabled = 1;
       irq_table[i].handler = NULL;
       irq_table[i].prv_args = NULL;
    }
}

int alloy_interrupt_set_pending(int irq, bool pending)
{
    if (irq >= MAX_MACHINE_INTERRUPTS)
    {
        return -1;
    }

    if (pending)
    {
        pending_mask |= 1 << irq;
    }
    else
    {
        pending_mask &= ~(1 << irq);
    }
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
        uint32_t mask = (1 << (i -1));

        // If not set previously, but it is now then trigger
        if (!(mask & _prev_pending) &&
            (mask & pending_mask))
        {
            alloy_interrupt_invoke(i - 1);
        }
    }

    _prev_pending = pending_mask;
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

int alloy_interrupt_set_enable(int irq, bool enable)
{
    if (irq >= MAX_MACHINE_INTERRUPTS)
    {
        return -1;
    }

    irq_table[irq].enabled = enable;
    return 0;
}
