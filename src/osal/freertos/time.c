/**
 * Copyright (c) 2021 Wavious LLC.
 *
 * SPDX-License-Identifier: Apache-2.0
 */

/* FreeRTOS includes. */
#include <FreeRTOS.h>
#include <task.h>

void osal_tick_periodic(void)
{
    if (xTaskIncrementTick())
    {
        vTaskSwitchContext();
    }
}
