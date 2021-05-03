/* Copyright 2019 SiFive, Inc */
/* SPDX-License-Identifier: Apache-2.0 */
/* ----------------------------------- */
/* ----------------------------------- */

#ifndef METAL_PLATFORM_H
#define METAL_PLATFORM_H

/* From refclk */
#define METAL_FIXED_CLOCK__CLOCK_FREQUENCY 38400000UL

#define METAL_FIXED_CLOCK

/* From clint@8000 */
#define METAL_RISCV_CLINT0_8000_BASE_ADDRESS 32768UL
#define METAL_RISCV_CLINT0_0_BASE_ADDRESS 32768UL
#define METAL_RISCV_CLINT0_8000_SIZE 32768UL
#define METAL_RISCV_CLINT0_0_SIZE 32768UL

#define METAL_RISCV_CLINT0
#define METAL_RISCV_CLINT0_MSIP_BASE 0UL
#define METAL_RISCV_CLINT0_MTIMECMP_BASE 36UL
#define METAL_RISCV_CLINT0_MTIME 28UL

#endif /* METAL_PLATFORM_H*/
