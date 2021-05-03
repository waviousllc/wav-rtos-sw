
/****************************************************************************
*****************************************************************************
** Wavious LLC Proprietary
**
** Copyright (c) 2019 Wavious LLC. All rights reserved.
**
** All data and information contained in or disclosed by this document
** are confidential and proprietary information of Wavious LLC,
** and all rights therein are expressly reserved. By accepting this
** material, the recipient agrees that this material and the information
** contained therein are held in confidence and in trust and will not be
** used, copied, reproduced in whole or in part, nor its contents
** revealed in any manner to others without the express written
** permission of Wavious LLC.
*****************************************************************************
** Author: schilukuri
** Date: 2021-01-14
** Version: 0.1
*****************************************************************************
****************************************************************************/
// MODULE MCU
// RANGE 0xFFFF:0x0000
// WIDTH 32

#ifndef _WAV_MCU_CSR_H_
#define _WAV_MCU_CSR_H_

// Word Address 0x00000000 : WAV_MCU_IRQ_FAST_CLR_CFG (WC)
#define WAV_MCU_IRQ_FAST_CLR_CFG_CLR__MSK                                                                    (  0xFFFFFFFF )
#define WAV_MCU_IRQ_FAST_CLR_CFG_CLR__SHFT                                                                   (  0x00000000 )
#define WAV_MCU_IRQ_FAST_CLR_CFG__ADR                                                                        (  0x00000000 )
#define WAV_MCU_IRQ_FAST_CLR_CFG__WIDTH                                                                      (  32 )
#define WAV_MCU_IRQ_FAST_CLR_CFG__POR                                                                        (  0x00000000 )
#define WAV_MCU_IRQ_FAST_CLR_CFG__MSK                                                                        (  0x00FFFFFF )


// Word Address 0x00000004 : WAV_MCU_IRQ_FAST_STICKY_CFG (RW)
#define WAV_MCU_IRQ_FAST_STICKY_CFG_EN__MSK                                                                  (  0xFFFFFFFF )
#define WAV_MCU_IRQ_FAST_STICKY_CFG_EN__SHFT                                                                 (  0x00000000 )
#define WAV_MCU_IRQ_FAST_STICKY_CFG__ADR                                                                     (  0x00000004 )
#define WAV_MCU_IRQ_FAST_STICKY_CFG__WIDTH                                                                   (  32 )
#define WAV_MCU_IRQ_FAST_STICKY_CFG__POR                                                                     (  0x003FFFFF )
#define WAV_MCU_IRQ_FAST_STICKY_CFG__MSK                                                                     (  0x00FFFFFF )


// Word Address 0x00000008 : WAV_MCU_IRQ_FAST_MSK_CFG (RW)
#define WAV_MCU_IRQ_FAST_MSK_CFG_MSK__MSK                                                                    (  0xFFFFFFFF )
#define WAV_MCU_IRQ_FAST_MSK_CFG_MSK__SHFT                                                                   (  0x00000000 )
#define WAV_MCU_IRQ_FAST_MSK_CFG__ADR                                                                        (  0x00000008 )
#define WAV_MCU_IRQ_FAST_MSK_CFG__WIDTH                                                                      (  32 )
#define WAV_MCU_IRQ_FAST_MSK_CFG__POR                                                                        (  0x00000000 )
#define WAV_MCU_IRQ_FAST_MSK_CFG__MSK                                                                        (  0x00FFFFFF )


// Word Address 0x0000000c : WAV_MCU_IRQ_FAST_SYNC_CFG (RW)
#define WAV_MCU_IRQ_FAST_SYNC_CFG_EN__MSK                                                                    (  0xFFFFFFFF )
#define WAV_MCU_IRQ_FAST_SYNC_CFG_EN__SHFT                                                                   (  0x00000000 )
#define WAV_MCU_IRQ_FAST_SYNC_CFG__ADR                                                                       (  0x0000000c )
#define WAV_MCU_IRQ_FAST_SYNC_CFG__WIDTH                                                                     (  32 )
#define WAV_MCU_IRQ_FAST_SYNC_CFG__POR                                                                       (  0x003FFFFF )
#define WAV_MCU_IRQ_FAST_SYNC_CFG__MSK                                                                       (  0x00FFFFFF )


// Word Address 0x00000010 : WAV_MCU_IRQ_FAST_EDGE_CFG (RW)
#define WAV_MCU_IRQ_FAST_EDGE_CFG_EN__MSK                                                                    (  0xFFFFFFFF )
#define WAV_MCU_IRQ_FAST_EDGE_CFG_EN__SHFT                                                                   (  0x00000000 )
#define WAV_MCU_IRQ_FAST_EDGE_CFG__ADR                                                                       (  0x00000010 )
#define WAV_MCU_IRQ_FAST_EDGE_CFG__WIDTH                                                                     (  32 )
#define WAV_MCU_IRQ_FAST_EDGE_CFG__POR                                                                       (  0x003FFFFF )
#define WAV_MCU_IRQ_FAST_EDGE_CFG__MSK                                                                       (  0x00FFFFFF )


// Word Address 0x00000014 : WAV_MCU_IRQ_FAST_STA (R)
#define WAV_MCU_IRQ_FAST_STA_IRQ_STA__MSK                                                                    (  0xFFFFFFFF )
#define WAV_MCU_IRQ_FAST_STA_IRQ_STA__SHFT                                                                   (  0x00000000 )
#define WAV_MCU_IRQ_FAST_STA__ADR                                                                            (  0x00000014 )
#define WAV_MCU_IRQ_FAST_STA__WIDTH                                                                          (  32 )
#define WAV_MCU_IRQ_FAST_STA__POR                                                                            (  0x00000000 )
#define WAV_MCU_IRQ_FAST_STA__MSK                                                                            (  0x00FFFFFF )


// Word Address 0x00000018 : WAV_MCU_MSIP_CFG (RW)
#define WAV_MCU_MSIP_CFG_INTERRUPT__MSK                                                                      (  0x00000001 )
#define WAV_MCU_MSIP_CFG_INTERRUPT__SHFT                                                                     (  0x00000000 )
#define WAV_MCU_MSIP_CFG__ADR                                                                                (  0x00000018 )
#define WAV_MCU_MSIP_CFG__WIDTH                                                                              (   1 )
#define WAV_MCU_MSIP_CFG__POR                                                                                (  0x00000000 )
#define WAV_MCU_MSIP_CFG__MSK                                                                                (  0x00000000 )


// Word Address 0x0000001c : WAV_MCU_MTIME_LO_STA (R)
#define WAV_MCU_MTIME_LO_STA_TIME_LO__MSK                                                                    (  0xFFFFFFFF )
#define WAV_MCU_MTIME_LO_STA_TIME_LO__SHFT                                                                   (  0x00000000 )
#define WAV_MCU_MTIME_LO_STA__ADR                                                                            (  0x0000001c )
#define WAV_MCU_MTIME_LO_STA__WIDTH                                                                          (  32 )
#define WAV_MCU_MTIME_LO_STA__POR                                                                            (  0x00000000 )
#define WAV_MCU_MTIME_LO_STA__MSK                                                                            (  0x00FFFFFF )


// Word Address 0x00000020 : WAV_MCU_MTIME_HI_STA (R)
#define WAV_MCU_MTIME_HI_STA_TIME_HI__MSK                                                                    (  0xFFFFFFFF )
#define WAV_MCU_MTIME_HI_STA_TIME_HI__SHFT                                                                   (  0x00000000 )
#define WAV_MCU_MTIME_HI_STA__ADR                                                                            (  0x00000020 )
#define WAV_MCU_MTIME_HI_STA__WIDTH                                                                          (  32 )
#define WAV_MCU_MTIME_HI_STA__POR                                                                            (  0x00000000 )
#define WAV_MCU_MTIME_HI_STA__MSK                                                                            (  0x00FFFFFF )


// Word Address 0x00000024 : WAV_MCU_MTIMECMP_LO_CFG (RW)
#define WAV_MCU_MTIMECMP_LO_CFG_TIMECMP_LO__MSK                                                              (  0xFFFFFFFF )
#define WAV_MCU_MTIMECMP_LO_CFG_TIMECMP_LO__SHFT                                                             (  0x00000000 )
#define WAV_MCU_MTIMECMP_LO_CFG__ADR                                                                         (  0x00000024 )
#define WAV_MCU_MTIMECMP_LO_CFG__WIDTH                                                                       (  32 )
#define WAV_MCU_MTIMECMP_LO_CFG__POR                                                                         (  0x00000000 )
#define WAV_MCU_MTIMECMP_LO_CFG__MSK                                                                         (  0x00FFFFFF )


// Word Address 0x00000028 : WAV_MCU_MTIMECMP_HI_CFG (RW)
#define WAV_MCU_MTIMECMP_HI_CFG_TIMECMP_HI__MSK                                                              (  0xFFFFFFFF )
#define WAV_MCU_MTIMECMP_HI_CFG_TIMECMP_HI__SHFT                                                             (  0x00000000 )
#define WAV_MCU_MTIMECMP_HI_CFG__ADR                                                                         (  0x00000028 )
#define WAV_MCU_MTIMECMP_HI_CFG__WIDTH                                                                       (  32 )
#define WAV_MCU_MTIMECMP_HI_CFG__POR                                                                         (  0x00000000 )
#define WAV_MCU_MTIMECMP_HI_CFG__MSK                                                                         (  0x00FFFFFF )


// Word Address 0x0000002c : WAV_MCU_MTIMECMP_CFG (W1T)
#define WAV_MCU_MTIMECMP_CFG_LOAD__MSK                                                                       (  0x00000001 )
#define WAV_MCU_MTIMECMP_CFG_LOAD__SHFT                                                                      (  0x00000000 )
#define WAV_MCU_MTIMECMP_CFG__ADR                                                                            (  0x0000002c )
#define WAV_MCU_MTIMECMP_CFG__WIDTH                                                                          (   1 )
#define WAV_MCU_MTIMECMP_CFG__POR                                                                            (  0x00000000 )
#define WAV_MCU_MTIMECMP_CFG__MSK                                                                            (  0x00000000 )


// Word Address 0x00000030 : WAV_MCU_MTIME_CFG (RW)
#define WAV_MCU_MTIME_CFG_ENABLE__MSK                                                                        (  0x00000001 )
#define WAV_MCU_MTIME_CFG_ENABLE__SHFT                                                                       (  0x00000000 )
#define WAV_MCU_MTIME_CFG__ADR                                                                               (  0x00000030 )
#define WAV_MCU_MTIME_CFG__WIDTH                                                                             (   1 )
#define WAV_MCU_MTIME_CFG__POR                                                                               (  0x00000001 )
#define WAV_MCU_MTIME_CFG__MSK                                                                               (  0x00000000 )


// Word Address 0x00000034 : WAV_MCU_GP0_CFG (RW)
#define WAV_MCU_GP0_CFG_RESERVED__MSK                                                                        (  0xFFFFFFFF )
#define WAV_MCU_GP0_CFG_RESERVED__SHFT                                                                       (  0x00000000 )
#define WAV_MCU_GP0_CFG__ADR                                                                                 (  0x00000034 )
#define WAV_MCU_GP0_CFG__WIDTH                                                                               (  32 )
#define WAV_MCU_GP0_CFG__POR                                                                                 (  0x00000000 )
#define WAV_MCU_GP0_CFG__MSK                                                                                 (  0x00FFFFFF )


// Word Address 0x00000038 : WAV_MCU_GP1_CFG (RW)
#define WAV_MCU_GP1_CFG_RESERVED__MSK                                                                        (  0xFFFFFFFF )
#define WAV_MCU_GP1_CFG_RESERVED__SHFT                                                                       (  0x00000000 )
#define WAV_MCU_GP1_CFG__ADR                                                                                 (  0x00000038 )
#define WAV_MCU_GP1_CFG__WIDTH                                                                               (  32 )
#define WAV_MCU_GP1_CFG__POR                                                                                 (  0x00000000 )
#define WAV_MCU_GP1_CFG__MSK                                                                                 (  0x00FFFFFF )


// Word Address 0x0000003c : WAV_MCU_GP2_CFG (RW)
#define WAV_MCU_GP2_CFG_RESERVED__MSK                                                                        (  0xFFFFFFFF )
#define WAV_MCU_GP2_CFG_RESERVED__SHFT                                                                       (  0x00000000 )
#define WAV_MCU_GP2_CFG__ADR                                                                                 (  0x0000003c )
#define WAV_MCU_GP2_CFG__WIDTH                                                                               (  32 )
#define WAV_MCU_GP2_CFG__POR                                                                                 (  0x00000000 )
#define WAV_MCU_GP2_CFG__MSK                                                                                 (  0x00FFFFFF )


// Word Address 0x00000040 : WAV_MCU_GP3_CFG (RW)
#define WAV_MCU_GP3_CFG_RESERVED__MSK                                                                        (  0xFFFFFFFF )
#define WAV_MCU_GP3_CFG_RESERVED__SHFT                                                                       (  0x00000000 )
#define WAV_MCU_GP3_CFG__ADR                                                                                 (  0x00000040 )
#define WAV_MCU_GP3_CFG__WIDTH                                                                               (  32 )
#define WAV_MCU_GP3_CFG__POR                                                                                 (  0x00000000 )
#define WAV_MCU_GP3_CFG__MSK                                                                                 (  0x00FFFFFF )


// Word Address 0x00000044 : WAV_MCU_DEBUG_CFG (RW)
#define WAV_MCU_DEBUG_CFG_INTR__MSK                                                                          (  0x00000001 )
#define WAV_MCU_DEBUG_CFG_INTR__SHFT                                                                         (  0x00000000 )
#define WAV_MCU_DEBUG_CFG_STEP__MSK                                                                          (  0x00000004 )
#define WAV_MCU_DEBUG_CFG_STEP__SHFT                                                                         (  0x00000002 )
#define WAV_MCU_DEBUG_CFG_EXIT__MSK                                                                          (  0x00000010 )
#define WAV_MCU_DEBUG_CFG_EXIT__SHFT                                                                         (  0x00000004 )
#define WAV_MCU_DEBUG_CFG__ADR                                                                               (  0x00000044 )
#define WAV_MCU_DEBUG_CFG__WIDTH                                                                             (   5 )
#define WAV_MCU_DEBUG_CFG__POR                                                                               (  0x00000000 )
#define WAV_MCU_DEBUG_CFG__MSK                                                                               (  0x00000000 )


// Word Address 0x00000048 : WAV_MCU_ITCM_CFG (RW)
#define WAV_MCU_ITCM_CFG_TIMING_PARAM__MSK                                                                   (  0x000000FF )
#define WAV_MCU_ITCM_CFG_TIMING_PARAM__SHFT                                                                  (  0x00000000 )
#define WAV_MCU_ITCM_CFG__ADR                                                                                (  0x00000048 )
#define WAV_MCU_ITCM_CFG__WIDTH                                                                              (   8 )
#define WAV_MCU_ITCM_CFG__POR                                                                                (  0x0000003F )
#define WAV_MCU_ITCM_CFG__MSK                                                                                (  0x00000000 )


// Word Address 0x0000004c : WAV_MCU_DTCM_CFG (RW)
#define WAV_MCU_DTCM_CFG_TIMING_PARAM__MSK                                                                   (  0x000000FF )
#define WAV_MCU_DTCM_CFG_TIMING_PARAM__SHFT                                                                  (  0x00000000 )
#define WAV_MCU_DTCM_CFG__ADR                                                                                (  0x0000004c )
#define WAV_MCU_DTCM_CFG__WIDTH                                                                              (   8 )
#define WAV_MCU_DTCM_CFG__POR                                                                                (  0x0000003F )
#define WAV_MCU_DTCM_CFG__MSK                                                                                (  0x00000000 )


#endif /* _WAV_MCU_CSR_H_ */
