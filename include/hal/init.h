/**
 * Copyright (c) 2021 Wavious LLC.
 *
 * SPDX-License-Identifier: Apache-2.0
 */
#ifndef _HAL_INIT_H_
#define _HAL_INIT_H_

#include <stdint.h>

/**
 * @brief   Hardware Abstraction Layer Subsystem Initialization
 *
 * @details Initializes the Hardware Abstraction Layer subsytem. This is a
 *          function that all Hardware Abstraction Layers must implement.
 *          Typically, this function will be called by the OSAL.
 *
 * @return  void.
 */
void hal_subsystem_init(void);

/**
 * @brief   Hardware Abstraction Layer Timer Init
 *
 * @details Initializes the CPU's timer via Hardware Abstraction Layer. This is
 *          a function that all Hardware Abstraction Layers must implement.
 *          Tyipcally, this function will be called by the OSAL.
 *
 * @return  void.
 */
void hal_timer_init(uint64_t timer_rate);

#endif /* _HAL_INIT_H_ */
