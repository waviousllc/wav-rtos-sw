/**
 * Copyright (c) 2021 Wavious LLC.
 *
 * SPDX-License-Identifier: Apache-2.0
 */
#ifndef _OSAL_QUEUE_H_
#define _OSAL_QUEUE_H_

void osal_queue_create();
void osal_queue_delete();
void osal_queue_enqueue_back();
void osal_queue_enqueue_back_fromISR();
void osal_queue_enqueue_front();
void osal_queue_enqueue_front_fromISR();
void osal_queue_get();

#endif /* _OSAL_QUEUE_H_ */
