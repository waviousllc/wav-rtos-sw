/**
 * Copyright (c) 2021 Wavious LLC.
 *
 * SPDX-License-Identifier: Apache-2.0
 */
#include <stdint.h>

#include "memory_map.h"

#ifndef WAV_MCU_IRQ_SYNC_CFG
    // Error out compilation
    #error WAV_MCU_IRQ_SYNC_CFG must be defined
    // Never used but defining so warnings below aren't generated
    #define WAV_MCU_IRQ_SYNC_CFG (0x00000000)
#endif
#ifndef WAV_MCU_IRQ_EDGE_CFG
    // Error out compilation
    #error WAV_MCU_IRQ_EDGE_CFG must be defined
    // Never used but defining so warnings below aren't generated
    #define WAV_MCU_IRQ_EDGE_CFG (0x00000000)
#endif

void board_init(void)
{
    volatile uint32_t * const edge_cfg_reg = (( volatile uint32_t * const )
                                             ( MEMORY_MAP_WAV_MCU + WAV_MCU_IRQ_FAST_EDGE_CFG__ADR ));
    volatile uint32_t * const sync_cfg_reg = (( volatile uint32_t * const )
                                              ( MEMORY_MAP_WAV_MCU + WAV_MCU_IRQ_FAST_SYNC_CFG__ADR ));

    *edge_cfg_reg = WAV_MCU_IRQ_EDGE_CFG;
    *sync_cfg_reg = WAV_MCU_IRQ_SYNC_CFG;

    // Set mtvec to custom interrupt vector
    // TODO: Need to decouple this from metal
    __asm__ __volatile__ (
        "la t0, __metal_vector_table\n"
        "csrw mtvec, t0"
    );
}
