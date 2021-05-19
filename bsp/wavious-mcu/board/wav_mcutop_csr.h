/**
 * Copyright (c) 2021 Wavious LLC.
 *
 * SPDX-License-Identifier: GPL-3.0
 */
// MODULE MCUTOP
// RANGE 0x03FF:0x0000
// WIDTH 32

#ifndef _WAV_MCUTOP_CSR_H_
#define _WAV_MCUTOP_CSR_H_

// Word Address 0x00000000 : WAV_MCUTOP_RSVD (RW)
#define WAV_MCUTOP_RSVD_RSVD__MSK                                                                            (  0xFFFFFFFF )
#define WAV_MCUTOP_RSVD_RSVD__SHFT                                                                           (  0x00000000 )
#define WAV_MCUTOP_RSVD__ADR                                                                                 (  0x00000000 )
#define WAV_MCUTOP_RSVD__WIDTH                                                                               (  32 )
#define WAV_MCUTOP_RSVD__POR                                                                                 (  0x00000000 )
#define WAV_MCUTOP_RSVD__MSK                                                                                 (  0x00FFFFFF )


// Word Address 0x00000004 : WAV_MCUTOP_CFG (RW)
#define WAV_MCUTOP_CFG_FETCH_EN__MSK                                                                         (  0x00000001 )
#define WAV_MCUTOP_CFG_FETCH_EN__SHFT                                                                        (  0x00000000 )
#define WAV_MCUTOP_CFG_DEBUG_EN__MSK                                                                         (  0x00000002 )
#define WAV_MCUTOP_CFG_DEBUG_EN__SHFT                                                                        (  0x00000001 )
#define WAV_MCUTOP_CFG__ADR                                                                                  (  0x00000004 )
#define WAV_MCUTOP_CFG__WIDTH                                                                                (   2 )
#define WAV_MCUTOP_CFG__POR                                                                                  (  0x00000000 )
#define WAV_MCUTOP_CFG__MSK                                                                                  (  0x00000000 )


// Word Address 0x00000008 : WAV_MCUTOP_STA (R)
#define WAV_MCUTOP_STA_RESERVED__MSK                                                                         (  0xFFFFFFFF )
#define WAV_MCUTOP_STA_RESERVED__SHFT                                                                        (  0x00000000 )
#define WAV_MCUTOP_STA__ADR                                                                                  (  0x00000008 )
#define WAV_MCUTOP_STA__WIDTH                                                                                (  32 )
#define WAV_MCUTOP_STA__POR                                                                                  (  0x00000000 )
#define WAV_MCUTOP_STA__MSK                                                                                  (  0x00FFFFFF )


#endif /* _WAV_MCUTOP_CSR_H_ */
