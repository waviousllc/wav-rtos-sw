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
__attribute__((always_inline))
static inline uint32_t hal_reg_read(uint64_t addr)
{
    uint64_t *reg = (uint64_t *) addr;
    return (uint32_t) (*reg & 0xFFFFFFFF);
}

/** @brief  Writes data to specified register address */
__attribute__((always_inline))
static inline void hal_reg_write(uint64_t addr, uint32_t data)
{
    uint64_t *reg = (uint64_t *) addr;
    *reg = data;
}
#endif /* _HAL_IO_H_ */
