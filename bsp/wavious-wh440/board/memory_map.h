/**
 * Copyright (c) 2021 Wavious LLC.
 *
 * SPDX-License-Identifier: Apache-2.0
 */
#ifndef _BOARD_MEMORY_MAP_H_
#define _BOARD_MEMORY_MAP_H_

// Local WH440
#define MEMORY_MAP_CLINT                    (0x02000000)
#define MEMORY_MAP_PLIC                     (0x0C000000)
#define MEMORY_MAP_UART                     (0x54000000)
#define MEMORY_MAP_ONCHIPSRAM               (0x60000000)
#define MEMORY_MAP_QSPI_FLASH_DIRECT_MEM    (0x70000000)
#define MEMORY_MAP_QSPI_FLASH               (0x90000000)
#define MEMORY_MAP_HBS                      (0xA0000000)
#define MEMORY_MAP_HBS_RPLL_CPUSS           (MEMORY_MAP_HBS + 0x100)
#define MEMORY_MAP_HBS_RPLL_SB              (MEMORY_MAP_HBS + 0x200)
#define MEMORY_MAP_HBS_RPLL_SD              (MEMORY_MAP_HBS + 0x300)
#define MEMORY_MAP_HBS_RPLL_AXI             (MEMORY_MAP_HBS + 0x400)

#define MTIME_CMP_BASE_ADDRESS              ( ( MEMORY_MAP_CLINT ) + 0x4000UL )
#define MTIME_BASE_ADDRESS_LO               ( ( MEMORY_MAP_CLINT ) + 0xBFF8UL )
#define MTIME_BASE_ADDRESS_HI               ( MTIME_BASE_ADDRESS_LO + 0x4 )
#define MTIME_CMP_ADDRESS(hart_id)          ( ( MTIME_CMP_BASE_ADDRESS + ( ulHartId * sizeof( uint64_t ) ) ) )

/**
 * CLINT in WH440 doesn't require an explicit reload. Its reloaded on write to
 * MTIME_CMP. HAL requires this MACRO to be defined so the same HAL
 * implementation is shared for all RISC-V boards. For WH440, it is defined as
 * empty so it resolves to nothing.
 */
#define MTIME_RELOAD()

#endif /* _BOARD_MEMORY_MAP_H_ */
