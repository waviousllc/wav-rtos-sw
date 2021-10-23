/**
 * Copyright (c) 2021 Wavious LLC.
 *
 * SPDX-License-Identifier: Apache-2.0
 */
#ifndef _BOARD_MEMORY_MAP_H_
#define _BOARD_MEMORY_MAP_H_

#include <stdint.h>

#include "wav_mcu_csr.h"

// Common MCU registers (same for all Wavious MCUs)
#define MEMORY_MAP_WAV_MCU_INTF         (0x00004000)
#define MEMORY_MAP_WAV_MCU              (0x00008000)
#define MEMORY_MAP_WAV_MCU_ITCM         (0x00010000)
#define MEMORY_MAP_WAV_MCU_DTCM         (0x00050000)

#define MTIME_BASE_ADDRESS_LO           ( ( MEMORY_MAP_WAV_MCU ) + ( WAV_MCU_MTIME_LO_STA__ADR ) )
#define MTIME_BASE_ADDRESS_HI           ( ( MEMORY_MAP_WAV_MCU ) + ( WAV_MCU_MTIME_HI_STA__ADR ) )
#define MTIME_CMP_ADDRESS(hart_id)      ( ( MEMORY_MAP_WAV_MCU ) + ( WAV_MCU_MTIMECMP_LO_CFG__ADR ) )

#define MTIME_RLD_REG                   ( (uint32_t) (( MEMORY_MAP_WAV_MCU ) + ( WAV_MCU_MTIMECMP_CFG__ADR )) )
#define MTIME_RELOAD()                  ( (*((uint32_t *)(MTIME_RLD_REG)) = 0x1) )

#endif /* _BOARD_MEMORY_MAP_H_ */
