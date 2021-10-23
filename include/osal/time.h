/**
 * Copyright (c) 2021 Wavious LLC.
 *
 * SPDX-License-Identifier: Apache-2.0
 */
#ifndef _OSAL_TIME_H_
#define _OSAL_TIME_H_

/**
 * @brief   Operating System Abstract Layer Tick Periodic
 *
 * @details The architecture independent part of the timer interrupt handler.
 *          This function should be called by the timer interrupt handler
 *          whenever the timer interrupt fires. Tyipcally, this is called
 *          by the Hardware Abstraction Layer.
 */
void osal_tick_periodic(void);

#endif /* _OSAL_TIME_H_ */
