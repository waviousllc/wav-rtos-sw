
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
// MODULE MCUINTF
// RANGE 0xFFFF:0x0000
// WIDTH 32

#ifndef _WAV_MCUINTF_CSR_H_
#define _WAV_MCUINTF_CSR_H_

// Word Address 0x00000000 : WAV_MCUINTF_HOST2MCU_MSG_DATA (RW)
#define WAV_MCUINTF_HOST2MCU_MSG_DATA_DATA__MSK                                                              (  0xFFFFFFFF )
#define WAV_MCUINTF_HOST2MCU_MSG_DATA_DATA__SHFT                                                             (  0x00000000 )
#define WAV_MCUINTF_HOST2MCU_MSG_DATA__ADR                                                                   (  0x00000000 )
#define WAV_MCUINTF_HOST2MCU_MSG_DATA__WIDTH                                                                 (  32 )
#define WAV_MCUINTF_HOST2MCU_MSG_DATA__POR                                                                   (  0x00000000 )
#define WAV_MCUINTF_HOST2MCU_MSG_DATA__MSK                                                                   (  0x00FFFFFF )


// Word Address 0x00000004 : WAV_MCUINTF_HOST2MCU_MSG_ID (RW)
#define WAV_MCUINTF_HOST2MCU_MSG_ID_ID__MSK                                                                  (  0xFFFFFFFF )
#define WAV_MCUINTF_HOST2MCU_MSG_ID_ID__SHFT                                                                 (  0x00000000 )
#define WAV_MCUINTF_HOST2MCU_MSG_ID__ADR                                                                     (  0x00000004 )
#define WAV_MCUINTF_HOST2MCU_MSG_ID__WIDTH                                                                   (  32 )
#define WAV_MCUINTF_HOST2MCU_MSG_ID__POR                                                                     (  0x00000000 )
#define WAV_MCUINTF_HOST2MCU_MSG_ID__MSK                                                                     (  0x00FFFFFF )


// Word Address 0x00000008 : WAV_MCUINTF_HOST2MCU_MSG_REQ (W1T)
#define WAV_MCUINTF_HOST2MCU_MSG_REQ_REQ__MSK                                                                (  0x00000001 )
#define WAV_MCUINTF_HOST2MCU_MSG_REQ_REQ__SHFT                                                               (  0x00000000 )
#define WAV_MCUINTF_HOST2MCU_MSG_REQ__ADR                                                                    (  0x00000008 )
#define WAV_MCUINTF_HOST2MCU_MSG_REQ__WIDTH                                                                  (   1 )
#define WAV_MCUINTF_HOST2MCU_MSG_REQ__POR                                                                    (  0x00000000 )
#define WAV_MCUINTF_HOST2MCU_MSG_REQ__MSK                                                                    (  0x00000000 )


// Word Address 0x0000000c : WAV_MCUINTF_HOST2MCU_MSG_ACK (W1T)
#define WAV_MCUINTF_HOST2MCU_MSG_ACK_ACK__MSK                                                                (  0x00000001 )
#define WAV_MCUINTF_HOST2MCU_MSG_ACK_ACK__SHFT                                                               (  0x00000000 )
#define WAV_MCUINTF_HOST2MCU_MSG_ACK__ADR                                                                    (  0x0000000c )
#define WAV_MCUINTF_HOST2MCU_MSG_ACK__WIDTH                                                                  (   1 )
#define WAV_MCUINTF_HOST2MCU_MSG_ACK__POR                                                                    (  0x00000000 )
#define WAV_MCUINTF_HOST2MCU_MSG_ACK__MSK                                                                    (  0x00000000 )


// Word Address 0x00000010 : WAV_MCUINTF_MCU2HOST_MSG_DATA (RW)
#define WAV_MCUINTF_MCU2HOST_MSG_DATA_DATA__MSK                                                              (  0xFFFFFFFF )
#define WAV_MCUINTF_MCU2HOST_MSG_DATA_DATA__SHFT                                                             (  0x00000000 )
#define WAV_MCUINTF_MCU2HOST_MSG_DATA__ADR                                                                   (  0x00000010 )
#define WAV_MCUINTF_MCU2HOST_MSG_DATA__WIDTH                                                                 (  32 )
#define WAV_MCUINTF_MCU2HOST_MSG_DATA__POR                                                                   (  0x00000000 )
#define WAV_MCUINTF_MCU2HOST_MSG_DATA__MSK                                                                   (  0x00FFFFFF )


// Word Address 0x00000014 : WAV_MCUINTF_MCU2HOST_MSG_ID (RW)
#define WAV_MCUINTF_MCU2HOST_MSG_ID_ID__MSK                                                                  (  0xFFFFFFFF )
#define WAV_MCUINTF_MCU2HOST_MSG_ID_ID__SHFT                                                                 (  0x00000000 )
#define WAV_MCUINTF_MCU2HOST_MSG_ID__ADR                                                                     (  0x00000014 )
#define WAV_MCUINTF_MCU2HOST_MSG_ID__WIDTH                                                                   (  32 )
#define WAV_MCUINTF_MCU2HOST_MSG_ID__POR                                                                     (  0x00000000 )
#define WAV_MCUINTF_MCU2HOST_MSG_ID__MSK                                                                     (  0x00FFFFFF )


// Word Address 0x00000018 : WAV_MCUINTF_MCU2HOST_MSG_REQ (W1T)
#define WAV_MCUINTF_MCU2HOST_MSG_REQ_REQ__MSK                                                                (  0x00000001 )
#define WAV_MCUINTF_MCU2HOST_MSG_REQ_REQ__SHFT                                                               (  0x00000000 )
#define WAV_MCUINTF_MCU2HOST_MSG_REQ__ADR                                                                    (  0x00000018 )
#define WAV_MCUINTF_MCU2HOST_MSG_REQ__WIDTH                                                                  (   1 )
#define WAV_MCUINTF_MCU2HOST_MSG_REQ__POR                                                                    (  0x00000000 )
#define WAV_MCUINTF_MCU2HOST_MSG_REQ__MSK                                                                    (  0x00000000 )


// Word Address 0x0000001c : WAV_MCUINTF_MCU2HOST_MSG_ACK (W1T)
#define WAV_MCUINTF_MCU2HOST_MSG_ACK_ACK__MSK                                                                (  0x00000001 )
#define WAV_MCUINTF_MCU2HOST_MSG_ACK_ACK__SHFT                                                               (  0x00000000 )
#define WAV_MCUINTF_MCU2HOST_MSG_ACK__ADR                                                                    (  0x0000001c )
#define WAV_MCUINTF_MCU2HOST_MSG_ACK__WIDTH                                                                  (   1 )
#define WAV_MCUINTF_MCU2HOST_MSG_ACK__POR                                                                    (  0x00000000 )
#define WAV_MCUINTF_MCU2HOST_MSG_ACK__MSK                                                                    (  0x00000000 )


#endif /* _WAV_MCUINTF_CSR_H_ */
