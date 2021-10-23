/**
 * Copyright (c) 2021 Wavious LLC.
 *
 * SPDX-License-Identifier: Apache-2.0
 */
#ifndef _HAL_IO_H_
#define _HAL_IO_H_

/* Standard includes. */
#include <stdint.h>

/** @brief  Returns data from specified register address */
#if __riscv_xlen == 64
__attribute__((always_inline))
static inline uint32_t hal_reg_read(uint64_t addr)
{
    uint32_t data;
    __asm__ volatile("lw %0, 0(%1)" : "=r" (data) : "r" ((volatile uint64_t *)addr));
    return data;
}
#elif __riscv_xlen == 32
__attribute__((always_inline))
static inline uint32_t hal_reg_read(uint32_t addr)
{
    uint32_t data;
    __asm__ volatile("lw %0, 0(%1)" : "=r" (data) : "r" ((volatile uint32_t *)addr));
    return data;
}
#else
    #error Assembler did not define __riscv_xlen
#endif

/** @brief  Writes data to specified register address */
#if __riscv_xlen == 64
__attribute__((always_inline))
static inline void hal_reg_write(uint64_t addr, uint32_t data)
{
    __asm__ volatile("sw %0, 0(%1)" : : "r" (data), "r" ((volatile uint64_t *)addr));
}
#elif __riscv_xlen == 32
__attribute__((always_inline))
static inline void hal_reg_write(uint32_t addr, uint32_t data)
{
    __asm__ volatile("sw %0, 0(%1)" : : "r" (data), "r" ((volatile uint32_t *)addr));
}
#else
    #error Assembler did not define __riscv_xlen
#endif

#endif /* _HAL_IO_H_ */
