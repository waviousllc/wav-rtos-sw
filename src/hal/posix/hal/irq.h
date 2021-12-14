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
}

/** @brief  Disables all interrupts */
__attribute__((always_inline))
static inline void hal_disable_interrupt_local(void)
{
}

/** @brief  Enables all interrupts */
__attribute__((always_inline))
static inline void hal_enable_interrupt_local(void)
{
}

/** @brief  Disable individual interrupts */
__attribute__((always_inline))
static inline void hal_interrupt_disable(uint32_t mask)
{
}

/** @brief  Enable individual interrupts */
__attribute__((always_inline))
static inline void hal_interrupt_enable(uint32_t mask)
{
}

/** @brief  Disable individual interrupts and return previous state */
__attribute__((always_inline))
static inline uint32_t hal_interrupt_disable_save(uint32_t mask, uint32_t flags)
{
    return flags;
}

/** @brief  Enable individual interrupts and return previous state */
__attribute__((always_inline))
static inline uint32_t hal_interrupt_enable_save(uint32_t mask, uint32_t flags)
{
    return flags;
}
/** @brief  Restore individual interrupts to previous state */
__attribute__((always_inline))
static inline void hal_interrupt_restore(uint32_t flags)
{
}

#endif /* _HAL_IRQ_H_ */
