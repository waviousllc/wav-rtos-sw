/**
 * Copyright (c) 2021 Wavious LLC.
 *
 * SPDX-License-Identifier: Apache-2.0
 */
#ifndef _KERNEL_IO_H_
#define _KERNEL_IO_H_

/* Standard includes. */
#include <stdint.h>

/* Hardware Abstraction Layer include. */
#include <hal/io.h>

/** Helper Macros for IO access */
#define SET_REG_FIELD(field_name, field_val)                ((field_val << field_name##__SHFT) & field_name##__MSK)
#define GET_REG_FIELD(reg_value, field_name)                ((reg_value & field_name##__MSK) >> field_name##__SHFT)
#define UPDATE_REG_FIELD(reg_value, field_name, field_val)  ((SET_REG_FIELD(field_name, field_val)) | (reg_value & ~field_name##__MSK))

#define GET_LO32(value)         ((value & 0xFFFFFFFF))
#define GET_HI32(value)         (GET_LO32(value >> 32))

#define reg_read                hal_reg_read
#define reg_write               hal_reg_write

#endif /* _KERNEL_IO_H_ */
