/**
 * Copyright (c) 2021 Wavious LLC.
 *
 * SPDX-License-Identifier: Apache-2.0
 */

/* Standard includes. */
#include <stdbool.h>
#include <stddef.h>

/* Kernel includes. */
#include <kernel/irq.h>

/* Alloy includes. */
#include <alloy/interrupt.h>

irq_return_t request_irq(uint32_t irq, hal_interrupt_handler_t handler, void *args)
{
    if (alloy_interrupt_register_handler(irq, handler, args) == 0)
    {
        return IRQ_RETURN_REGISTERED;
    }

    return IRQ_RETURN_INVALID_IRQ_NUM;
}

irq_return_t free_irq(uint32_t irq)
{
    if (alloy_interrupt_register_handler(irq, NULL, NULL) == 0)
    {
        return IRQ_RETURN_REGISTERED;
    }

    return IRQ_RETURN_INVALID_IRQ_NUM;
}

int8_t enable_irq(uint32_t irq)
{
    return alloy_interrupt_set_enable(irq, true);
}

int8_t disable_irq(uint32_t irq)
{
    return alloy_interrupt_set_enable(irq, false);
}
