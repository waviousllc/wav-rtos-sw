/* Copyright 2019 SiFive, Inc */
/* SPDX-License-Identifier: Apache-2.0 */
/* Modified by waviousllc */

/* Standard includes. */
#include <stdio.h>
#include <string.h>
#include <unistd.h>

/* FreeRTOS kernel includes. */
#include <FreeRTOS.h>
#include <task.h>

/* Hardware Abstraction Layer includes. */
#include <hal/init.h>

#define TIMER_INC_ONE_TICK  ((size_t) ( ( configCPU_CLOCK_HZ ) / ( configTICK_RATE_HZ ) ))

__attribute__ ((aligned(4))) uint8_t ucHeap[ configTOTAL_HEAP_SIZE ] __attribute__ ((section (".heap")));

__attribute__((constructor)) static void FreeRTOS_init(void);

#if ( configUSE_TIMERS == 1 )

void vPortSetupTimerInterrupt( void )
{
    hal_timer_init(TIMER_INC_ONE_TICK);
}
#endif /* configUSE_TIMERS == 1 */

__attribute__((constructor)) static void FreeRTOS_init(void)
{
    hal_subsystem_init();
}

void _putchar(char character)
{
    write(STDOUT_FILENO, &character, 1);
}
