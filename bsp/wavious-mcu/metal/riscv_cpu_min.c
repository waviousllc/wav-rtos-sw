/**
 * Copyright (c) 2021 Wavious LLC.
 *
 * SPDX-License-Identifier: Apache-2.0
 */
#include <metal/machine.h>
#include <metal/drivers/riscv_cpu_min.h>

/*******************************************************************************
**                   INTERRUPT CONTROLLER VTABLE
*******************************************************************************/
static void __metal_driver_riscv_cpu_controller_interrupt_init(struct metal_interrupt *controller)
{
    struct __metal_driver_riscv_cpu_min_intc *intc = (void *)(controller);
    if (!intc->init_done)
    {
        // Clear out table
        for (uint8_t i = 0; i < METAL_MAX_MI; i++)
        {
            intc->metal_int_table[i].handler = NULL;
            intc->metal_int_table[i].sub_int = NULL;
            intc->metal_int_table[i].exint_data = NULL;
        }
    }
    intc->init_done = 1;
}

static int __metal_driver_riscv_cpu_controller_interrupt_register(struct metal_interrupt *controller,
                                                                  int id,
                                                                  metal_interrupt_handler_t isr,
                                                                  void *priv)
{
    struct __metal_driver_riscv_cpu_min_intc *intc = (void *)(controller);

    if (id >= METAL_MAX_MI)
    {
        return -11;
    }

    intc->metal_int_table[id].handler = isr;
    intc->metal_int_table[id].exint_data = priv;
    return 0;
}

static int __metal_driver_riscv_cpu_controller_interrupt_enable(struct metal_interrupt *controller,
                                                                int id)
{
    uint32_t mask;
    if (id >= METAL_MAX_MI)
    {
        return -1;
    }

    mask = 1 << id;
    __asm__ volatile("csrs mie, %0" : : "r" (mask));
    return 0;
}

static int __metal_driver_riscv_cpu_controller_interrupt_disable(struct metal_interrupt *controller,
                                                                 int id)
{
    uint32_t mask;
    if (id >= METAL_MAX_MI)
    {
        return -1;
    }

    mask = 1 << id;
    __asm__ volatile("csrc mie, %0" : : "r" (mask));
    return 0;
}

static metal_vector_mode __metal_driver_riscv_cpu_controller_get_vector_mode(struct metal_interrupt *controller)
{
    uintptr_t val;
    __asm__ volatile("csrr %0, mtvec" : "=r"(val));
    val &= 0x1;

    if (val == METAL_MTVEC_VECTORED)
    {
        return METAL_VECTOR_MODE;
    }

    return METAL_DIRECT_MODE;
}

static int __metal_driver_riscv_cpu_controller_set_vector_mode(struct metal_interrupt *controller,
                                                               metal_vector_mode mode)
{
    // Not supported
    return -1;
}

static int __metal_driver_riscv_cpu_controller_command_request(struct metal_interrupt *controller,
                                                               int cmd,
                                                               void *data)
{
    // Not supported
    return 0;
}

/*******************************************************************************
**                          CPU VTABLE
*******************************************************************************/
static struct metal_interrupt * __metal_driver_cpu_controller_interrupt(struct metal_cpu *cpu)
{
    return __metal_driver_cpu_interrupt_controller(cpu);
}

static int __metal_driver_cpu_get_timer_interrupt_id(struct metal_cpu *cpu)
{
    return METAL_INTERRUPT_ID_TMR;
}

static int __metal_driver_cpu_get_sw_interrupt_id(struct metal_cpu *cpu)
{
    return METAL_INTERRUPT_ID_SW;
}

__METAL_DEFINE_VTABLE(__metal_driver_vtable_riscv_cpu_min_intc) = {
    .controller_vtable.interrupt_init =
        __metal_driver_riscv_cpu_controller_interrupt_init,
    .controller_vtable.interrupt_register =
        __metal_driver_riscv_cpu_controller_interrupt_register,
    .controller_vtable.interrupt_enable =
        __metal_driver_riscv_cpu_controller_interrupt_enable,
    .controller_vtable.interrupt_disable =
        __metal_driver_riscv_cpu_controller_interrupt_disable,
    .controller_vtable.interrupt_get_vector_mode =
        __metal_driver_riscv_cpu_controller_get_vector_mode,
    .controller_vtable.interrupt_set_vector_mode =
        __metal_driver_riscv_cpu_controller_set_vector_mode,
    .controller_vtable.command_request =
        __metal_driver_riscv_cpu_controller_command_request,
};

__METAL_DEFINE_VTABLE(__metal_driver_vtable_cpu_min) = {
    .cpu_vtable.mcycle_get = NULL,
    .cpu_vtable.timebase_get = NULL,
    .cpu_vtable.mtime_get = NULL,
    .cpu_vtable.mtimecmp_set = NULL,
    .cpu_vtable.tmr_controller_interrupt = NULL,
    .cpu_vtable.get_tmr_interrupt_id = __metal_driver_cpu_get_timer_interrupt_id,
    .cpu_vtable.sw_controller_interrupt = NULL,
    .cpu_vtable.get_sw_interrupt_id = __metal_driver_cpu_get_sw_interrupt_id,
    .cpu_vtable.set_sw_ipi = NULL,
    .cpu_vtable.clear_sw_ipi = NULL,
    .cpu_vtable.get_msip = NULL,
    .cpu_vtable.controller_interrupt = __metal_driver_cpu_controller_interrupt,
    .cpu_vtable.exception_register = NULL,
    .cpu_vtable.get_ilen = NULL,
    .cpu_vtable.get_epc = NULL,
    .cpu_vtable.set_epc = NULL,
    .cpu_vtable.get_buserror = NULL,
};
