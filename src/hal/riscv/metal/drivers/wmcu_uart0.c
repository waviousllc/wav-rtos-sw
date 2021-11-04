/* Copyright 2018 SiFive, Inc */
/* SPDX-License-Identifier: Apache-2.0 */
/* Modified by wavious */

#include <metal/machine/platform.h>

#ifdef METAL_WMCU_UART0
#include <metal/drivers/wmcu_uart0.h>
#include <metal/io.h>
#include <metal/machine.h>

// Internal Register struct
struct reg_node
{
    uint32_t offset;
    struct reg_node *next;
};

// GP0 through GP3
static struct reg_node reg_list[3] =
{
    {.offset = METAL_WMCU_UART0_GP0, .next = &reg_list[1]},
    {.offset = METAL_WMCU_UART0_GP1, .next = &reg_list[2]},
    {.offset = METAL_WMCU_UART0_GP2, .next = &reg_list[0]},
};

#define UART_REG(address) (((unsigned long)address))
#define UART_REGW(address)                                                     \
    (__METAL_ACCESS_ONCE((__metal_io_u32 *)UART_REG(address)))

void __metal_driver_wmcu_uart0_init(struct metal_uart *guart, int baud_rate)
{
    struct __metal_driver_wmcu_uart0 *uart = (struct __metal_driver_wmcu_uart0 *) guart;
    uart->next = (void *) &reg_list[0];
}

int __metal_driver_wmcu_uart0_putc(struct metal_uart *guart, int c)
{
    struct reg_node *next;
    long control_base = __metal_driver_wmcu_uart0_control_base(guart);
    struct __metal_driver_wmcu_uart0 *uart = (struct __metal_driver_wmcu_uart0 *) guart;
    next = (struct reg_node *) uart->next;

    // Save next register
    uart->next = (void *) next->next;
    UART_REGW(control_base + next->offset) = c;
    return 0;
}

int __metal_driver_wmcu_uart0_txready(struct metal_uart *guart)
{
    // Always ready
    return 0;
}

int __metal_driver_wmcu_uart0_getc(struct metal_uart *guart, int *c)
{
    // GETC not supported
    *c = -1;
    return 0;
}

__METAL_DEFINE_VTABLE(__metal_driver_vtable_wmcu_uart0) = {
    .uart.init = __metal_driver_wmcu_uart0_init,
    .uart.putc = __metal_driver_wmcu_uart0_putc,
    .uart.getc = __metal_driver_wmcu_uart0_getc,
    .uart.txready = __metal_driver_wmcu_uart0_txready,
    .uart.get_baud_rate = NULL,
    .uart.set_baud_rate = NULL,
    .uart.controller_interrupt = NULL,
    .uart.get_interrupt_id = NULL,
    .uart.tx_interrupt_enable = NULL,
    .uart.tx_interrupt_disable = NULL,
    .uart.rx_interrupt_enable = NULL,
    .uart.rx_interrupt_disable = NULL,
    .uart.set_tx_watermark = NULL,
    .uart.get_tx_watermark = NULL,
    .uart.set_rx_watermark = NULL,
    .uart.get_rx_watermark = NULL,
};

#endif /* METAL_WMCU_UART0 */
