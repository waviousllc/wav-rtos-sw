/* Copyright 2018 SiFive, Inc */
/* SPDX-License-Identifier: Apache-2.0 */
/* Modified by wavious */

#ifndef METAL__DRIVERS__WMCU_UART0_H
#define METAL__DRIVERS__WMCU_UART0_H

#include <metal/clock.h>
#include <metal/compiler.h>
#include <metal/uart.h>

struct __metal_driver_vtable_wmcu_uart0 {
    const struct metal_uart_vtable uart;
};

struct __metal_driver_wmcu_uart0;

__METAL_DECLARE_VTABLE(__metal_driver_vtable_wmcu_uart0)

struct __metal_driver_wmcu_uart0 {
    struct metal_uart uart;
    void *next;
};

#endif
