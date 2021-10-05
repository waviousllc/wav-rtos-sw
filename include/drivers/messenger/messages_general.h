/**
 * Copyright (c) 2021 Wavious LLC.
 *
 * SPDX-License-Identifier: Apache-2.0
 */
#ifndef _MESSAGES_GENERAL_H_
#define _MESSAGES_GENERAL_H_

/**
 * @brief   General Message Enumeration
 *
 * @details These messages are supported by all devices.
 *
 * RESERVED         unused/reserved message.
 * INTF_READY_RESP  messaging interface ready response.
 * FW_VER_REQ       firmware version request.
 * FW_VER_RESP      firmware version response.
 * HW_VER_REQ       hardware version request.
 * HW_VER_RESP      hardware version response.
 * MCU_BOOT_REQ     MCU complete boot request.
 * MCU_BOOT_RESP    MCU complete boot response.
 * END_OF_MESSAGES  indicates number of general messages.
 */
typedef enum messages_general_t {
    MESSAGE_GENERAL_RESERVED,
    MESSAGE_GENERAL_INTF_READY_RESP,
    MESSAGE_GENERAL_FW_VER_REQ,
    MESSAGE_GENERAL_FW_VER_RESP,
    MESSAGE_GENERAL_HW_VER_REQ,
    MESSAGE_GENERAL_HW_VER_RESP,
    MESSAGE_GENERAL_MCU_BOOT_REQ,
    MESSAGE_GENERAL_MCU_BOOT_RESP,
    MESSAGE_GENERAL_END_OF_MESSAGES,
} messages_general_t;

#define GENERAL_INTF_READY_RESP__MCU_DEVICE_ID__MSK         (0x0000FFFF)
#define GENERAL_INTF_READY_RESP__MCU_DEVICE_ID__SHFT        (0x00000000)
#define GENERAL_INTF_READY_RESP__MCU_DEVICE_REV__MSK        (0xFFFF0000)
#define GENERAL_INTF_READY_RESP__MCU_DEVICE_REV__SHFT       (0x00000010)

#define GENERAL_FW_VER_RESP__PATCH__MSK                     (0x000000FF)
#define GENERAL_FW_VER_RESP__PATCH__SHFT                    (0x00000000)
#define GENERAL_FW_VER_RESP__MINOR__MSK                     (0x0000FF00)
#define GENERAL_FW_VER_RESP__MINOR__SHFT                    (0x00000008)
#define GENERAL_FW_VER_RESP__MAJOR__MSK                     (0x00FF0000)
#define GENERAL_FW_VER_RESP__MAJOR__SHFT                    (0x00000010)

#define GENERAL_HW_VER_RESP__MINOR__MSK                     (0x0000FFFF)
#define GENERAL_HW_VER_RESP__MINOR__SHFT                    (0x00000000)
#define GENERAL_HW_VER_RESP__MAJOR__MSK                     (0xFFFF0000)
#define GENERAL_HW_VER_RESP__MAJOR__SHFT                    (0x00000010)

#define GENERAL_MCU_BOOT_RESP__STATUS__MSK                  (0x000000FF)
#define GENERAL_MCU_BOOT_RESP__STATUS__SHFT                 (0x00000000)
#define GENERAL_MCU_BOOT_RESP__CODE__MSK                    (0x00FFFF00)
#define GENERAL_MCU_BOOT_RESP__CODE__SHFT                   (0x00000008)

#endif /* _MESSAGES_GENERAL_H_ */
