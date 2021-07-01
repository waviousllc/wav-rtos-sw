/**
 * Copyright (c) 2021 Wavious LLC.
 *
 * SPDX-License-Identifier: Apache-2.0
 */
/* Standard includes. */
#include <stdio.h>
#include <string.h>
#include <unistd.h>

/* Image header includes. */
#include <image.h>

/* Kernel includes. */
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"

/* Freedom metal includes. */
#include <metal/machine.h>
#include <metal/machine/platform.h>

#include <metal/lock.h>
#include <metal/uart.h>
#include <metal/interrupt.h>
#include <metal/clock.h>

/*-----------------------------------------------------------*/
extern uint32_t __start;
img_hdr_t image_hdr __attribute__((section(".image_hdr"))) = {
    .image_magic = IMAGE_MAGIC,
    .image_hdr_version = IMAGE_VERSION_CURRENT,
    .image_type = IMAGE_TYPE_APP,
    .version_major = 1,
    .version_minor = 0,
    .version_patch = 0,
    .vector_addr = (uint32_t) &__start,
    .device_id = IMAGE_DEVICE_ID_HOST,
    .git_dirty = GIT_DIRTY,
    .git_ahead = GIT_AHEAD,
    .git_sha = GIT_SHA,
    // populated as part of a post compilation step
    .crc = 0,
    .data_size = 0,
};

/*-----------------------------------------------------------*/
/*
 * Functions:
 *      - prvSetupHardware: Setup Hardware according CPU and Board.
 */
static void prvSetupHardware( void );

static uint32_t task1_DELAY_LOOP_COUNT = 100;
static uint32_t task2_DELAY_LOOP_COUNT = 200;

static void vWriteTask1( void *pvParameters );
static void vWriteTask2( void *pvParameters );

/*-----------------------------------------------------------*/
int main( void )
{
    const char * const pcMessage = "FreeRTOS Demo start\r\n";
    const char * const pcMessageEnd = "FreeRTOS Demo end\r\n";

    prvSetupHardware();
    write( STDOUT_FILENO, pcMessage, strlen( pcMessage ) );

    /* At this point, you can create queue,semaphore, task requested for your application */
    xTaskCreate( vWriteTask1, "Task 1", configMINIMAL_STACK_SIZE, NULL, 2, NULL );
    xTaskCreate( vWriteTask2, "Task 2", configMINIMAL_STACK_SIZE, NULL, 2, NULL );

    /* Start the tasks and timer running. */
    /* Here No task are defined, so if we start the Scheduler 2 tasks will running (Timer and Idle) */
    /* This task will run forever without doing nothing, it is why the following line is commented */
    vTaskStartScheduler();

    /* If all is well, the scheduler will now be running, and the following
    line will never be reached.  If the following line does execute, then
    there was insufficient FreeRTOS heap memory available for the Idle and/or
    timer tasks to be created.
    or task have stoppped the Scheduler */

    write( STDOUT_FILENO, pcMessageEnd, strlen( pcMessageEnd ) );
}

static void vWriteTask1( void *pvParameters )
{
    const char * const taskMessage = "Task 1 Running\r\n";
    volatile uint32_t ul;
    for( ;; )
    {
        write (STDOUT_FILENO, taskMessage, strlen(taskMessage));

        for( ul = 0; ul < task1_DELAY_LOOP_COUNT; ul ++ );
    }
}

static void vWriteTask2( void *pvParameters )
{
    const char * const taskMessage = "Task 2 Running\r\n";
    volatile uint32_t ul;
    for( ;; )
    {
        write (STDOUT_FILENO, taskMessage, strlen(taskMessage));

        for( ul = 0; ul < task2_DELAY_LOOP_COUNT; ul ++ );
    }
}

/*-----------------------------------------------------------*/
static void prvSetupHardware( void )
{
    const char * const pcMsg = "prvSetupHardware\n";
    write( STDOUT_FILENO, pcMsg, strlen( pcMsg ) );
    // TODO: Inits here
}
/*-----------------------------------------------------------*/


void vApplicationMallocFailedHook( void )
{
    /* vApplicationMallocFailedHook() will only be called if
    configUSE_MALLOC_FAILED_HOOK is set to 1 in FreeRTOSConfig.h.  It is a hook
    function that will get called if a call to pvPortMalloc() fails.
    pvPortMalloc() is called internally by the kernel whenever a task, queue,
    timer or semaphore is created.  It is also called by various parts of the
    demo application.  If heap_1.c or heap_2.c are used, then the size of the
    heap available to pvPortMalloc() is defined by configTOTAL_HEAP_SIZE in
    FreeRTOSConfig.h, and the xPortGetFreeHeapSize() API function can be used
    to query the size of free heap space that remains (although it does not
    provide information on how the remaining heap might be fragmented). */
    taskDISABLE_INTERRUPTS();
    _exit(1);
}
/*-----------------------------------------------------------*/

void vApplicationIdleHook( void )
{
    /* vApplicationIdleHook() will only be called if configUSE_IDLE_HOOK is set
    to 1 in FreeRTOSConfig.h.  It will be called on each iteration of the idle
    task.  It is essential that code added to this hook function never attempts
    to block in any way (for example, call xQueueReceive() with a block time
    specified, or call vTaskDelay()).  If the application makes use of the
    vTaskDelete() API function (as this demo application does) then it is also
    important that vApplicationIdleHook() is permitted to return to its calling
    function, because it is the responsibility of the idle task to clean up
    memory allocated by the kernel to any task that has since been deleted. */
}
/*-----------------------------------------------------------*/

void vApplicationStackOverflowHook( TaskHandle_t pxTask, char *pcTaskName )
{
    ( void ) pcTaskName;
    ( void ) pxTask;

    /* Run time stack overflow checking is performed if
    configCHECK_FOR_STACK_OVERFLOW is defined to 1 or 2.  This hook
    function is called if a stack overflow is detected. */
    taskDISABLE_INTERRUPTS();

    write( STDOUT_FILENO, "ERROR Stack overflow on func: ", 30 );
    write( STDOUT_FILENO, pcTaskName, strlen( pcTaskName ) );
    _exit(1);
}
/*-----------------------------------------------------------*/

void vApplicationTickHook( void )
{
    /* The tests in the full demo expect some interaction with interrupts. */
}
/*-----------------------------------------------------------*/

void vAssertCalled( void )
{
    taskDISABLE_INTERRUPTS();
    _exit(1);
}
