/**
 * Copyright (c) 2021 Wavious LLC.
 *
 * SPDX-License-Identifier: Apache-2.0
 */
#ifndef _ALLOY_INTERRUPT_H_
#define _ALLOY_INTERRUPT_H_
#include <stdbool.h>

typedef void (*alloy_interrupt_handler_t)(int, void *);

void alloy_interrupt_tick(void);

int alloy_interrupt_register_handler(int irq,
                                     alloy_interrupt_handler_t handler,
                                     void *args);

int alloy_interrupt_set_pending(int irq, bool pending);
int alloy_interrupt_set_enable(int irq, bool enable);

#endif /* _ALLOY_INTERRUPT_H_ */
