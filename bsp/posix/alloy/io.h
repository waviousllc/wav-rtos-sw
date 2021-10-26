/**
 * Copyright (c) 2021 Wavious LLC.
 *
 * SPDX-License-Identifier: Apache-2.0
 */
#ifndef _ALLOY_IO_H_
#define _ALLOY_IO_H_

#include <stdint.h>

uint32_t alloy_reg_read(uint64_t base);
void alloy_reg_write(uint64_t base, uint32_t data);

#endif /* _ALLOY_IO_H_ */
