/* Copyright 2019 SiFive, Inc */
/* SPDX-License-Identifier: Apache-2.0 */
/* Modified by waviousllc */
/* ----------------------------------- */
/* ----------------------------------- */

#ifndef ASSEMBLY

#include <metal/machine/platform.h>

#ifdef __METAL_MACHINE_MACROS

#ifndef MACROS_IF_METAL_H
#define MACROS_IF_METAL_H

#define __METAL_CLINT_NUM_PARENTS 2

#ifndef __METAL_CLINT_NUM_PARENTS
#define __METAL_CLINT_NUM_PARENTS 0
#endif
#ifndef __METAL_PLIC_SUBINTERRUPTS
#define __METAL_PLIC_SUBINTERRUPTS 0
#endif
#ifndef __METAL_PLIC_NUM_PARENTS
#define __METAL_PLIC_NUM_PARENTS 0
#endif
#ifndef __METAL_CLIC_SUBINTERRUPTS
#define __METAL_CLIC_SUBINTERRUPTS 0
#endif

#endif /* MACROS_IF_METAL_H*/

#else /* ! __METAL_MACHINE_MACROS */

#ifndef MACROS_ELSE_METAL_H
#define MACROS_ELSE_METAL_H

#define __METAL_CLINT_8000_INTERRUPTS 2

#define METAL_MAX_CLINT_INTERRUPTS 2

#define __METAL_CLINT_NUM_PARENTS 2

#define __METAL_PLIC_SUBINTERRUPTS 0
#define METAL_MAX_PLIC_INTERRUPTS 0

#define __METAL_PLIC_NUM_PARENTS 0

#define __METAL_CLIC_SUBINTERRUPTS 0
#define METAL_MAX_CLIC_INTERRUPTS 0

#define METAL_MAX_LOCAL_EXT_INTERRUPTS 0

#define METAL_MAX_GLOBAL_EXT_INTERRUPTS 0

#define METAL_MAX_GPIO_INTERRUPTS 0

#define METAL_MAX_I2C0_INTERRUPTS 0

#define METAL_MAX_PWM0_INTERRUPTS 0

#define METAL_MAX_PWM0_NCMP 0

#define METAL_MAX_UART_INTERRUPTS 0

#define METAL_MAX_SIMUART_INTERRUPTS 0


#include <metal/drivers/fixed-clock.h>
#include <metal/memory.h>
#include <metal/drivers/riscv_clint0.h>
#include <metal/drivers/riscv_cpu_min.h>
#include <metal/drivers/wav_test0.h>
#include <metal/drivers/wmcu_uart0.h>
#include <metal/pmp.h>

/* From refclk */
extern struct __metal_driver_fixed_clock __metal_dt_refclk;

extern struct metal_memory __metal_dt_mem_dtim_50000;

extern struct metal_memory __metal_dt_mem_itim_10000;

/* From clint@8000 */
extern struct __metal_driver_riscv_clint0 __metal_dt_clint_8000;

/* From cpu@0 */
extern struct __metal_driver_cpu_min __metal_dt_cpu_0;

extern struct __metal_driver_riscv_cpu_min_intc __metal_dt_cpu_0_interrupt_controller;

extern struct metal_pmp __metal_dt_pmp;

/* From test@8000 */
extern struct __metal_driver_wav_test0 __metal_dt_test_8000;

extern struct __metal_driver_wmcu_uart0 __metal_dt_serial_8000;

/* --------------------- fixed_clock ------------ */
static __inline__ unsigned long __metal_driver_fixed_clock_rate(const struct metal_clock *clock)
{
    if ((uintptr_t)clock == (uintptr_t)&__metal_dt_refclk) {
        return METAL_FIXED_CLOCK__CLOCK_FREQUENCY;
    }
    else {
        return 0;
    }
}



/* --------------------- fixed_factor_clock ------------ */


/* --------------------- sifive_clint0 ------------ */
static __inline__ unsigned long __metal_driver_sifive_clint0_control_base(struct metal_interrupt *controller)
{
    if ((uintptr_t)controller == (uintptr_t)&__metal_dt_clint_8000) {
        return METAL_RISCV_CLINT0_8000_BASE_ADDRESS;
    }
    else {
        return 0;
    }
}

static __inline__ unsigned long __metal_driver_sifive_clint0_control_size(struct metal_interrupt *controller)
{
    if ((uintptr_t)controller == (uintptr_t)&__metal_dt_clint_8000) {
        return METAL_RISCV_CLINT0_8000_SIZE;
    }
    else {
        return 0;
    }
}

static __inline__ int __metal_driver_sifive_clint0_num_interrupts(struct metal_interrupt *controller)
{
    if ((uintptr_t)controller == (uintptr_t)&__metal_dt_clint_8000) {
        return METAL_MAX_CLINT_INTERRUPTS;
    }
    else {
        return 0;
    }
}

static __inline__ struct metal_interrupt * __metal_driver_sifive_clint0_interrupt_parents(struct metal_interrupt *controller, int idx)
{
    if (idx == 0) {
        return (struct metal_interrupt *)&__metal_dt_cpu_0_interrupt_controller.controller;
    }
    else if (idx == 1) {
        return (struct metal_interrupt *)&__metal_dt_cpu_0_interrupt_controller.controller;
    }
    else {
        return NULL;
    }
}

static __inline__ int __metal_driver_sifive_clint0_interrupt_lines(struct metal_interrupt *controller, int idx)
{
    if (idx == 0) {
        return 3;
    }
    else if (idx == 1) {
        return 7;
    }
    else {
        return 0;
    }
}



/* --------------------- cpu ------------ */
static __inline__ int __metal_driver_cpu_hartid(struct metal_cpu *cpu)
{
    if ((uintptr_t)cpu == (uintptr_t)&__metal_dt_cpu_0) {
        return 0;
    }
    else {
        return -1;
    }
}

static __inline__ int __metal_driver_cpu_timebase(struct metal_cpu *cpu)
{
    if ((uintptr_t)cpu == (uintptr_t)&__metal_dt_cpu_0) {
        return 1000000;
    }
    else {
        return 0;
    }
}

static __inline__ struct metal_interrupt * __metal_driver_cpu_interrupt_controller(struct metal_cpu *cpu)
{
    if ((uintptr_t)cpu == (uintptr_t)&__metal_dt_cpu_0) {
        return &__metal_dt_cpu_0_interrupt_controller.controller;
    }
    else {
        return NULL;
    }
}

static __inline__ int __metal_driver_cpu_num_pmp_regions(struct metal_cpu *cpu)
{
    if ((uintptr_t)cpu == (uintptr_t)&__metal_dt_cpu_0) {
        return 8;
    }
    else {
        return 0;
    }
}

static __inline__ struct metal_buserror * __metal_driver_cpu_buserror(struct metal_cpu *cpu)
{
    if ((uintptr_t)cpu == (uintptr_t)&__metal_dt_cpu_0) {
        return NULL;
    }
    else {
        return NULL;
    }
}



/* --------------------- sifive_plic0 ------------ */


/* --------------------- sifive_buserror0 ------------ */


/* --------------------- sifive_clic0 ------------ */


/* --------------------- sifive_local_external_interrupts0 ------------ */


/* --------------------- sifive_global_external_interrupts0 ------------ */


/* --------------------- sifive_gpio0 ------------ */


/* --------------------- sifive_gpio_button ------------ */


/* --------------------- sifive_gpio_led ------------ */


/* --------------------- sifive_gpio_switch ------------ */


/* --------------------- sifive_i2c0 ------------ */


/* --------------------- sifive_pwm0 ------------ */


/* --------------------- sifive_rtc0 ------------ */



/* --------------------- wav_test0 ------------ */
static __inline__ unsigned long __metal_driver_wav_test0_base(const struct __metal_shutdown *sd)
{
	if ((uintptr_t)sd == (uintptr_t)&__metal_dt_test_8000) {
		return METAL_WAV_TEST0_8000_BASE_ADDRESS;
	}
	else {
		return 0;
	}
}

/* --------------------- sifive_trace ------------ */

/* --------------------- wmcu_uart0 ------------ */
static __inline__ unsigned long __metal_driver_wmcu_uart0_control_base(struct metal_uart *uart)
{
    if ((uintptr_t)uart == (uintptr_t)&__metal_dt_serial_8000) {
        return METAL_WMCU_UART0_8000_BASE_ADDRESS;
    }
    else {
        return 0;
    }
}

static __inline__ unsigned long __metal_driver_wmcu_uart0_control_size(struct metal_uart *uart)
{
    if ((uintptr_t)uart == (uintptr_t)&__metal_dt_serial_8000) {
        return METAL_WMCU_UART0_8000_SIZE;
    }
    else {
        return 0;
    }
}

/* --------------------- sifive_simuart0 ------------ */


/* --------------------- sifive_wdog0 ------------ */


/* --------------------- sifive_fe310_g000_hfrosc ------------ */


/* --------------------- sifive_fe310_g000_hfxosc ------------ */


/* --------------------- sifive_fe310_g000_lfrosc ------------ */


/* --------------------- sifive_fe310_g000_pll ------------ */


/* --------------------- sifive_fe310_g000_prci ------------ */


#define __METAL_DT_MAX_MEMORIES 2

__asm__ (".weak __metal_memory_table");
struct metal_memory *__metal_memory_table[] = {
                    &__metal_dt_mem_dtim_50000,
                    &__metal_dt_mem_itim_10000};

#define __METAL_DT_STDOUT_UART_HANDLE (&__metal_dt_serial_8000.uart)

/* From clint@8000 */
#define __METAL_DT_RISCV_CLINT0_HANDLE (&__metal_dt_clint_8000.controller)

#define __METAL_DT_CLINT_8000_HANDLE (&__metal_dt_clint_8000.controller)

#define __METAL_DT_MAX_HARTS 1

__asm__ (".weak __metal_cpu_table");
struct __metal_driver_cpu_min *__metal_cpu_table[] = {
                    &__metal_dt_cpu_0};

#define __METAL_DT_PMP_HANDLE (&__metal_dt_pmp)

#define __MEE_DT_MAX_GPIOS 0

__asm__ (".weak __metal_gpio_table");
struct __metal_driver_sifive_gpio0 *__metal_gpio_table[] = {
                    NULL };
#define __METAL_DT_MAX_BUTTONS 0

__asm__ (".weak __metal_button_table");
struct __metal_driver_sifive_gpio_button *__metal_button_table[] = {
                    NULL };
#define __METAL_DT_MAX_LEDS 0

__asm__ (".weak __metal_led_table");
struct __metal_driver_sifive_gpio_led *__metal_led_table[] = {
                    NULL };
#define __METAL_DT_MAX_SWITCHES 0

__asm__ (".weak __metal_switch_table");
struct __metal_driver_sifive_gpio_switch *__metal_switch_table[] = {
                    NULL };
#define __METAL_DT_MAX_I2CS 0

__asm__ (".weak __metal_i2c_table");
struct __metal_driver_sifive_i2c0 *__metal_i2c_table[] = {
                    NULL };
#define __METAL_DT_MAX_PWMS 0

__asm__ (".weak __metal_pwm_table");
struct __metal_driver_sifive_pwm0 *__metal_pwm_table[] = {
                    NULL };
#define __METAL_DT_MAX_RTCS 0

__asm__ (".weak __metal_rtc_table");
struct __metal_driver_sifive_rtc0 *__metal_rtc_table[] = {
                    NULL };
#define __METAL_DT_MAX_SPIS 0

__asm__ (".weak __metal_spi_table");
struct __metal_driver_sifive_spi0 *__metal_spi_table[] = {
                    NULL };
#define __METAL_DT_MAX_UARTS 0

#define __METAL_DT_SHUTDOWN_HANDLE (&__metal_dt_test_8000.shutdown)

__asm__ (".weak __metal_uart_table");
struct __metal_driver_wmcu_uart0 *__metal_uart_table[] = {
                    &__metal_dt_serial_8000};
#define __METAL_DT_MAX_SIMUARTS 0

__asm__ (".weak __metal_simuart_table");
struct __metal_driver_sifive_simuart0 *__metal_simuart_table[] = {
                    NULL };
#define __METAL_DT_MAX_WDOGS 0

__asm__ (".weak __metal_wdog_table");
struct __metal_driver_sifive_wdog0 *__metal_wdog_table[] = {
                    NULL };
#endif /* MACROS_ELSE_METAL_H*/

#endif /* ! __METAL_MACHINE_MACROS */

#endif /* ! ASSEMBLY */
