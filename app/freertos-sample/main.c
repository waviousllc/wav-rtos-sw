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

/* FreeRTOS includes. */
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "timers.h"

#include <kernel/printf.h>

/*-----------------------------------------------------------*/
/* Don't include image header for POSIX based builds */
#ifndef _POSIX_VERSION
extern uintptr_t __start;
img_hdr_t image_hdr __attribute__((section(".image_hdr"))) = {
    .image_magic = IMAGE_MAGIC,
    .image_hdr_version = IMAGE_VERSION_CURRENT,
    .image_type = IMAGE_TYPE_APP,
    .version_major = 1,
    .version_minor = 0,
    .version_patch = 0,
    .vector_size = VECTOR_SIZE,
    .vector_addr = (uintptr_t) &__start,
    .device_id = IMAGE_DEVICE_ID_HOST,
    .git_dirty = GIT_DIRTY,
    .git_ahead = GIT_AHEAD,
    .git_sha = GIT_SHA,
    // populated as part of a post compilation step
    .crc = 0,
    .data_size = 0,
};
#endif /* !_POSIX_VERSION */

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
static void vShutdown( TimerHandle_t xTimer);

/*-----------------------------------------------------------*/
int main( void )
{
    static TimerHandle_t xTimer;
    prvSetupHardware();
    configPRINTF(("FreeRTOS Demo Start\n"));

    /* At this point, you can create queue,semaphore, task requested for your application */
    xTaskCreate( vWriteTask1, "Task 1", configMINIMAL_STACK_SIZE, NULL, 2, NULL );
    xTaskCreate( vWriteTask2, "Task 2", configMINIMAL_STACK_SIZE, NULL, 2, NULL );

    // Create shutdown timer
    xTimer = xTimerCreate("Shtudown",
                          pdMS_TO_TICKS(2),
                          pdFALSE,
                          NULL,
                          vShutdown);

    if (xTimerStart(xTimer, 0) == pdFAIL)
    {
        configPRINTF(("Failed to start timer\n"));
        _exit(1);
    }

    /* Start the tasks and timer running. */
    /* Here No task are defined, so if we start the Scheduler 2 tasks will running (Timer and Idle) */
    /* This task will run forever without doing nothing, it is why the following line is commented */
    vTaskStartScheduler();

    /* If all is well, the scheduler will now be running, and the following
    line will never be reached.  If the following line does execute, then
    there was insufficient FreeRTOS heap memory available for the Idle and/or
    timer tasks to be created.
    or task have stoppped the Scheduler */

    configPRINTF(("FreeRTOS Demo end"));
}

static void vWriteTask1( void *pvParameters )
{
    volatile uint32_t ul;
    for( ;; )
    {
        configPRINTF(("Task 1 Running\n"));

        for( ul = 0; ul < task1_DELAY_LOOP_COUNT; ul ++ );
    }
}

static void vWriteTask2( void *pvParameters )
{
    volatile uint32_t ul;
    for( ;; )
    {

        configPRINTF(("Task 2 Running\n"));

        for( ul = 0; ul < task2_DELAY_LOOP_COUNT; ul ++ );
    }
}

static void vShutdown( TimerHandle_t xTimer)
{
    (void) xTimer;
    configPRINTF(("Shutdown\n"));
    _exit(0);
}

/*-----------------------------------------------------------*/
static void prvSetupHardware( void )
{
    configPRINTF(("prvSetupHardware\n"));
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
    configPRINTF(("ERROR: Malloc failed.\n"));
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
    configPRINTF(("ERROR: Stack overflow in task: %s.", pcTaskName));
    _exit(1);
}
/*-----------------------------------------------------------*/

void vApplicationTickHook( void )
{
    /* The tests in the full demo expect some interaction with interrupts. */
}

/*-----------------------------------------------------------*/
void vAssertCalled( const char * const pcFileName, unsigned long ulLine )
{
    const char *pcString = pcFileName;
    uint32_t ulFileNameLen = strlen(pcFileName);
    char cFileName[7] = {'\0'};

    // Extract filename
    memcpy(&cFileName[0], &pcString[ulFileNameLen - 6], 6);

    // Ignore assertions from port.c
    if (!strcmp(&cFileName[0], "port.c"))
    {
        return;
    }

    configPRINTF(("ERROR: Aserrtion in %s on line %lu.\n", pcFileName, ulLine));
    taskDISABLE_INTERRUPTS();
    _exit(1);
}
