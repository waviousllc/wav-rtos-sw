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

#endif /* _MESSAGES_GENERAL_H_ */
