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

irq_return_t request_irq(uint32_t irq, irq_handler_t handler, void *args)
{
    return IRQ_RETURN_REGISTERED;
}

irq_return_t free_irq(uint32_t irq)
{
    return IRQ_RETURN_REGISTERED;
}

int8_t enable_irq(uint32_t irq)
{
    return 0;
}

int8_t disable_irq(uint32_t irq)
{
    return 0;
}
