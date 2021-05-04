#ifndef _MEMORY_MAP_H_
#define _MEMORY_MAP_H_

// Local WH440
#define ONCHIPSRAM_ADDR                 (0x60000000)
#define CLINT_ADDR                      (0x02000000)
#define PLIC_ADDR                       (0x0C000000)
#define QSPI_FLASH_ADDR                 (0x90000000)
#define QSPI_FLASH_DIRECT_MEM_ADDR      (0x70000000)
#define UART_ADDR                       (0x54000000)
#define DRAM_ADDR                       (0x1000000000ull)

// HBS
#define HBS_ADDR                        (0xA0000000)
// HBS RPLLs
#define RPLL_CPUSS_OFFSET               (0x100)
#define RPLL_SB_OFFSET                  (0x200)
#define RPLL_SD_OFFSET                  (0x300)
#define RPLL_AXI_OFFSET                 (0x400)

// WTMs
#define WTM_BASE                        (0x000100000000)
#define WTM_OFFSET                      (0x000100000000)
#define WTM_TOTAL_NUMBER                (4)
#define WTM_WMCU_OFFSET                 (0x00000000)
#define WTM_CFG_OFFSET                  (0x0012f000)
#define WTM_CFG_SLV_OFFSET              (0x0022F000)
#define WTM_CSR_CHIPLET_BASE            (0x000080000000)
#define WTM0_ADDR                       (WTM_BASE)
#define WTM1_ADDR                       (WTM_BASE + WTM_OFFSET)
#define WTM2_ADDR                       (WTM_BASE + 2 * WTM_OFFSET)
#define WTM3_ADDR                       (WTM_BASE + 3 * WTM_OFFSET)

// WMCU
#define WMCU_ITCM_OFFSET                (0x00010000)
#define WMCU_DTCM_OFFSET                (0x00050000)
#define WMCU_INTF_OFFSET                (0x00004000)

#endif /* _MEMORY_MAP_H_ */
