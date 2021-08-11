/**
 * Copyright (c) 2021 Wavious LLC.
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#ifndef METAL__DRIVERS__RISCV_CPU_MIN_H
#define METAL__DRIVERS__RISCV_CPU_MIN_H
#include <metal/drivers/riscv_cpu.h>

/* CPU interrupt controller */
struct __metal_driver_vtable_riscv_cpu_min_intc {
    struct metal_interrupt_vtable controller_vtable;
};

__METAL_DECLARE_VTABLE(__metal_driver_vtable_riscv_cpu_min_intc)

struct __metal_driver_riscv_cpu_min_intc {
    struct metal_interrupt controller;
    int init_done;
    __metal_interrupt_data metal_int_table[METAL_MAX_MI];
};

struct __metal_driver_vtable_cpu_min {
    struct metal_cpu_vtable cpu_vtable;
};

__METAL_DECLARE_VTABLE(__metal_driver_vtable_cpu_min)

struct __metal_driver_cpu_min {
    struct metal_cpu cpu;
};

#endif /* METAL__DRIVERS__RISCV_CPU_MIN_H */
