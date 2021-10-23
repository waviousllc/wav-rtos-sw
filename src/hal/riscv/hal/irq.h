/**
 * Copyright (c) 2021 Wavious LLC.
 *
 * SPDX-License-Identifier: Apache-2.0
 */
#ifndef _HAL_IRQ_H_
#define _HAL_IRQ_H_

/* Standard includes. */
#include <stdint.h>

/** @brief  Halt CPU until interrupt fires */
__attribute__((always_inline))
static inline void hal_wait_for_interrupt(void)
{
    __asm__ volatile("wfi");
}

/** @brief  Disables all interrupts */
__attribute__((always_inline))
static inline void hal_disable_interrupt_local(void)
{
    __asm__ volatile("csrc mstatus, 0x8");
}

/** @brief  Enables all interrupts */
__attribute__((always_inline))
static inline void hal_enable_interrupt_local(void)
{
    __asm__ volatile("csrs mstatus, 0x8");
}

/** @brief  Disable individual interrupts */
__attribute__((always_inline))
static inline void hal_interrupt_disable(uint32_t mask)
{
    __asm__ volatile("csrc mie, %0" : : "r" (mask));
}

/** @brief  Enable individual interrupts */
__attribute__((always_inline))
static inline void hal_interrupt_enable(uint32_t mask)
{
    __asm__ volatile("csrs mie, %0" : : "r" (mask));
}

/** @brief  Disable individual interrupts and return previous state */
__attribute__((always_inline))
static inline uint32_t hal_interrupt_disable_save(uint32_t mask, uint32_t flags)
{
    __asm__ volatile ("csrrc %0, mie, %1" : "=r" (flags) : "r" (mask));
    return flags;
}

/** @brief  Enable individual interrupts and return previous state */
__attribute__((always_inline))
static inline uint32_t hal_interrupt_enable_save(uint32_t mask, uint32_t flags)
{
    __asm__ volatile ("csrrs %0, mie, %1" : "=r" (flags) : "r" (mask));
    return flags;
}
/** @brief  Restore individual interrupts to previous state */
__attribute__((always_inline))
static inline void hal_interrupt_restore(uint32_t flags)
{
    __asm__ volatile("csrw mie, %0" : : "r" (flags));
}

#endif /* _HAL_IRQ_H_ */
