/**
 * Copyright (c) 2021 Wavious LLC.
 *
 * SPDX-License-Identifier: Apache-2.0
 */
#ifndef _OSAL_THREAD_H_
#define _OSAL_THREAD_H_

void osal_thread_create();
void osal_thread_delete();
void osal_thread_suspend();
void osal_thread_resume();
void osal_thread_scheduler_start();
void osal_thread_get_current();

#endif /* _OSAL_THREAD_H_ */
