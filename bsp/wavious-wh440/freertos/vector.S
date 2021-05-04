/* Copyright 2019 SiFive, Inc */
/* SPDX-License-Identifier: Apache-2.0 */
/* Modified by waviousllc */

.global freertos_risc_v_trap_handler

/*
 * Jump table for CLINT vectored mode
 */
.weak metal_interrupt_vector_handler
.balign 4, 0
metal_interrupt_vector_handler:
  nop

.weak metal_software_interrupt_vector_handler
.balign 4, 0
metal_software_interrupt_vector_handler:
  nop

.weak metal_timer_interrupt_vector_handler
.balign 4, 0
metal_timer_interrupt_vector_handler:
  nop

.weak metal_external_interrupt_vector_handler
.balign 4, 0
metal_external_interrupt_vector_handler:
  nop

.weak metal_lc0_interrupt_vector_handler
.balign 4, 0
metal_lc0_interrupt_vector_handler:
  nop

.weak metal_lc1_interrupt_vector_handler
.balign 4, 0
metal_lc1_interrupt_vector_handler:
  nop

.weak metal_lc2_interrupt_vector_handler
.balign 4, 0
metal_lc2_interrupt_vector_handler:
  nop

.weak metal_lc3_interrupt_vector_handler
.balign 4, 0
metal_lc3_interrupt_vector_handler:
  nop

.weak metal_lc4_interrupt_vector_handler
.balign 4, 0
metal_lc4_interrupt_vector_handler:
  nop

.weak metal_lc5_interrupt_vector_handler
.balign 4, 0
metal_lc5_interrupt_vector_handler:
  nop

.weak metal_lc6_interrupt_vector_handler
.balign 4, 0
metal_lc6_interrupt_vector_handler:
  nop

.weak metal_lc7_interrupt_vector_handler
.balign 4, 0
metal_lc7_interrupt_vector_handler:
  nop

.weak metal_lc8_interrupt_vector_handler
.balign 4, 0
metal_lc8_interrupt_vector_handler:
  nop

.weak metal_lc9_interrupt_vector_handler
.balign 4, 0
metal_lc9_interrupt_vector_handler:
  nop

.weak metal_lc10_interrupt_vector_handler
.balign 4, 0
metal_lc10_interrupt_vector_handler:
  nop

.weak metal_lc11_interrupt_vector_handler
.balign 4, 0
metal_lc11_interrupt_vector_handler:
  nop

.weak metal_lc12_interrupt_vector_handler
.balign 4, 0
metal_lc12_interrupt_vector_handler:
  nop

.weak metal_lc13_interrupt_vector_handler
.balign 4, 0
metal_lc13_interrupt_vector_handler:
  nop

.weak metal_lc14_interrupt_vector_handler
.balign 4, 0
metal_lc14_interrupt_vector_handler:
  nop

.weak metal_lc15_interrupt_vector_handler
.balign 4, 0
metal_lc15_interrupt_vector_handler:
  nop
