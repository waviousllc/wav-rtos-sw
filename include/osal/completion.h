/**
 * Copyright (c) 2021 Wavious LLC.
 *
 * SPDX-License-Identifier: Apache-2.0
 */
#ifndef _OSAL_COMPLETION_H_
#define _OSAL_COMPLETION_H_

void osal_completion_init();
void osal_completion_reinit();
void osal_completion_complete();
void osal_completion_complete_fromISR();
void osal_completion_wait();
void osal_completion_wait_timeout();

#endif /* _OSAL_COMPLETION_H_ */

