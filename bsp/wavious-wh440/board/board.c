/**
 * Copyright (c) 2021 Wavious LLC.
 *
 * SPDX-License-Identifier: Apache-2.0
 */
void board_init(void)
{
    // Setup MTVEC
    // TODO: Decouple from FreeRTOS
    __asm__ __volatile__ (
            "la t0, freertos_risc_v_trap_handler\n"
            "csrw mtvec, t0"
    );
}
