/**
 * Copyright (c) 2021 Wavious LLC.
 *
 * SPDX-License-Identifier: Apache-2.0
 */
#ifndef _KERNEL_IO_H_
#define _KERNEL_IO_H_

/* Standard includes. */
#include <stdint.h>

/** Helper Macros for IO access */
#define SET_REG_FIELD(field_name, field_val)                ((field_val << field_name##__SHFT) & field_name##__MSK)
#define GET_REG_FIELD(reg_value, field_name)                ((reg_value & field_name##__MSK) >> field_name##__SHFT)
#define UPDATE_REG_FIELD(reg_value, field_name, field_val)  ((SET_REG_FIELD(field_name, field_val)) | (reg_value & ~field_name##__MSK))

#define GET_LO32(value)         ((value & 0xFFFFFFFF))
#define GET_HI32(value)         (GET_LO32(value >> 32))

/** @brief  Returns data from specified register address */
#if __riscv_xlen == 64
__attribute__((always_inline))
static inline uint32_t reg_read(uint64_t addr)
{
    uint32_t data;
    __asm__ volatile("lw %0, 0(%1)" : "=r" (data) : "r" ((volatile uint64_t *)addr));
    return data;
}
#elif __riscv_xlen == 32
__attribute__((always_inline))
static inline uint32_t reg_read(uint32_t addr)
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
static inline void reg_write(uint64_t addr, uint32_t data)
{
    __asm__ volatile("sw %0, 0(%1)" : : "r" (data), "r" ((volatile uint64_t *)addr));
}
#elif __riscv_xlen == 32
__attribute__((always_inline))
static inline void reg_write(uint32_t addr, uint32_t data)
{
    __asm__ volatile("sw %0, 0(%1)" : : "r" (data), "r" ((volatile uint32_t *)addr));
}
#else
    #error Assembler did not define __riscv_xlen
#endif

#endif /* _KERNEL_IO_H_ */
