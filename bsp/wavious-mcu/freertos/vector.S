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

.section .trap_handler_vector
.balign 256, 0
.option norvc
.global __metal_vector_table
__metal_vector_table:
IRQ_0:
        j freertos_risc_v_trap_handler
IRQ_1:
        j freertos_risc_v_trap_handler
IRQ_2:
        j freertos_risc_v_trap_handler
IRQ_3:
        j freertos_risc_v_trap_handler
IRQ_4:
        j freertos_risc_v_trap_handler
IRQ_5:
        j freertos_risc_v_trap_handler
IRQ_6:
        j freertos_risc_v_trap_handler
IRQ_7:
        j freertos_risc_v_trap_handler
IRQ_8:
        j freertos_risc_v_trap_handler
IRQ_9:
        j freertos_risc_v_trap_handler
IRQ_10:
        j freertos_risc_v_trap_handler
IRQ_11:
        j freertos_risc_v_trap_handler
IRQ_12:
        j freertos_risc_v_trap_handler
IRQ_13:
        j freertos_risc_v_trap_handler
IRQ_14:
        j freertos_risc_v_trap_handler
IRQ_15:
        j freertos_risc_v_trap_handler
IRQ_LC0:
        j freertos_risc_v_trap_handler
IRQ_LC1:
        j freertos_risc_v_trap_handler
IRQ_LC2:
        j freertos_risc_v_trap_handler
IRQ_LC3:
        j freertos_risc_v_trap_handler
IRQ_LC4:
        j freertos_risc_v_trap_handler
IRQ_LC5:
        j freertos_risc_v_trap_handler
IRQ_LC6:
        j freertos_risc_v_trap_handler
IRQ_LC7:
        j freertos_risc_v_trap_handler
IRQ_LC8:
        j freertos_risc_v_trap_handler
IRQ_LC9:
        j freertos_risc_v_trap_handler
IRQ_LC10:
        j freertos_risc_v_trap_handler
IRQ_LC11:
        j freertos_risc_v_trap_handler
IRQ_LC12:
        j freertos_risc_v_trap_handler
IRQ_LC13:
        j freertos_risc_v_trap_handler
IRQ_LC14:
        j freertos_risc_v_trap_handler
IRQ_LC15:
        j freertos_risc_v_trap_handler

.section .boot
__jump_to_start:
    j __start
