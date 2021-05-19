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

/* Metal includes. */
#include <metal/cpu.h>

/** @brief Interrupt Controller ID */
#define INTC_ID     (0)

static inline struct metal_interrupt * get_interrupt_controller( void )
{
#ifdef METAL_RISCV_PLIC0
    return metal_interrupt_get_controller(METAL_PLIC_CONTROLLER, INTC_ID);
#else
    struct metal_cpu *cpu = metal_cpu_get(metal_cpu_get_current_hartid());
    return metal_cpu_interrupt_controller(cpu);
#endif
}

static irq_return_t irq_register_internal(struct metal_interrupt *intc,
                                          uint32_t irq,
                                          metal_interrupt_handler_t handler,
                                          void *args)
{
    if (!intc)
    {
        return IRQ_RETURN_ERROR;
    }

    // Register with Interrupt Controller
    if(metal_interrupt_register_handler(intc, irq, handler, args))
    {
        return IRQ_RETURN_INVALID_IRQ_NUM;
    }

    return IRQ_RETURN_REGISTERED;
}

irq_return_t request_irq(uint32_t irq, metal_interrupt_handler_t handler, void *args)
{
    irq_return_t ret;
    struct metal_interrupt *intc = get_interrupt_controller();

    ret = irq_register_internal(intc, irq, handler, args);
    if (ret)
    {
        return ret;
    }

    if (metal_interrupt_enable(intc, irq) < 0)
    {
        return IRQ_RETURN_INVALID_IRQ_NUM;
    }

    return IRQ_RETURN_REGISTERED;
}

irq_return_t free_irq(uint32_t irq)
{
    irq_return_t ret;
    struct metal_interrupt *intc = get_interrupt_controller();

    ret = irq_register_internal(intc, irq, NULL, NULL);
    if (ret)
    {
        return ret;
    }
    if (metal_interrupt_disable(intc, irq) < 0)
    {
        return IRQ_RETURN_INVALID_IRQ_NUM;
    }

    return IRQ_RETURN_REGISTERED;
}

int8_t enable_irq(uint32_t irq)
{
    struct metal_interrupt *intc = get_interrupt_controller();

    if (metal_interrupt_enable(intc, irq) < 0)
    {
        return -1;
    }

    return 0;
}

int8_t disable_irq(uint32_t irq)
{
    struct metal_interrupt *intc = get_interrupt_controller();

    if (metal_interrupt_disable(intc, irq) < 0)
    {
        return  -1;
    }

    return 0;
}
