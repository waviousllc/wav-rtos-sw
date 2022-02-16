/**
 * Copyright (c) 2021 Wavious LLC.
 *
 * SPDX-License-Identifier: Apache-2.0
 */
#ifndef _OSAL_SEMAPHORE_H_
#define _OSAL_SEMAPHORE_H_

void osal_semaphore_create();
void osal_semaphore_post();
void osal_semaphore_post_fromISR();
void osal_semaphore_wait();
void osal_semaphore_wait_fromISR();

#endif /* _OSAL_SEMAPHORE_H_ */
