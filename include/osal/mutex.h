/**
 * Copyright (c) 2021 Wavious LLC.
 *
 * SPDX-License-Identifier: Apache-2.0
 */
#ifndef _OSAL_MUTEX_H_
#define _OSAL_MUTEX_H_

void osal_mutex_init();
void osal_mutex_lock();
void osal_mutex_try_lock();
void osal_mutex_unlock();

#endif /* _OSAL_MUTEX_H_ */

