/**
 * Copyright (c) 2021 Wavious LLC.
 *
 * SPDX-License-Identifier: Apache-2.0
 */
#ifndef _MEMORY_MAP_H_
#define _MEMORY_MAP_H_

// Local WH440
#define MEMORY_MAP_CLINT                    (0x02000000)
#define MEMORY_MAP_PLIC                     (0x0C000000)
#define MEMORY_MAP_UART                     (0x54000000)
#define MEMORY_MAP_ONCHIPSRAM               (0x60000000)
#define MEMORY_MAP_QSPI_FLASH_DIRECT_MEM    (0x70000000)
#define MEMORY_MAP_WTM_CSR                  (0x80000000)
#define MEMORY_MAP_QSPI_FLASH               (0x90000000)
#define MEMORY_MAP_HBS                      (0xA0000000)
#define MEMORY_MAP_HBS_RPLL_CPUSS           (MEMORY_MAP_HBS + 0x100)
#define MEMORY_MAP_HBS_RPLL_SB              (MEMORY_MAP_HBS + 0x200)
#define MEMORY_MAP_HBS_RPLL_SD              (MEMORY_MAP_HBS + 0x300)
#define MEMORY_MAP_HBS_RPLL_AXI             (MEMORY_MAP_HBS + 0x400)

#endif /* _MEMORY_MAP_H_ */
