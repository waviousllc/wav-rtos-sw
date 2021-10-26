/**
 * Test Main - Application harness for all tests
 *
 * Copyright (c) 2021 Wavious LLC.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 * SPDX-License-Identifier: Apache-2.0
 */

/* Standard includes. */
#include <stdio.h>
#include <unistd.h>
#include <string.h>

/* FreeRTOS includes. */
#include <FreeRTOS.h>
#include <task.h>

/* Image header includes. */
#include <image.h>

/* Kernel includes. */
#include <kernel/messenger.h>
#include <board/board.h>

/* Messenger includes. */
#include <messenger/driver.h>
#include <messenger/messages_general.h>

/*******************************************************************************
**                            FUNCTION DECLARATIONS
*******************************************************************************/
/** @brief  Centralized shutdown function */
static void shutdown(uint32_t cause);
static void vMainTask( void *pvParameters );

/*******************************************************************************
**                           VARIABLE DECLARATIONS
*******************************************************************************/
img_hdr_t image_hdr __attribute__((section(".image_hdr"))) = {
    .image_magic = IMAGE_MAGIC,
    .image_hdr_version = IMAGE_VERSION_CURRENT,
    .image_type = IMAGE_TYPE_APP,
    .version_major = 1,
    .version_minor = 0,
    .version_patch = 0,
    .device_id = IMAGE_DEVICE_ID_HOST,
    .git_dirty = GIT_DIRTY,
    .git_ahead = GIT_AHEAD,
    .git_sha = GIT_SHA,
    // populated as part of a post compilation step
    .crc = 0,
    .data_size = 0,
};

/*******************************************************************************
**                              IMPLEMENTATIONS
*******************************************************************************/
static bool validate_message(void *dev, void *buff, size_t len)
{
    return true;
}

int main( void )
{
    xTaskCreate( vMainTask, "Main Task", configMINIMAL_STACK_SIZE, NULL, 2, NULL );
    xMessengerTaskInit(3, configMINIMAL_STACK_SIZE, 1);

    // NOTE: This must always be called to include necessary FreeRTOS code
    vTaskStartScheduler();

    // Test failed
    shutdown(0x01);
}

/*-----------------------------------------------------------*/
static void vMainTask( void *pvParameters )
{
    message_interface_t messenger;
    Message_t message;

    // Initialize messenger interface
    if(!xMessengerInterfaceRegisterPhyInterface((void *) &messenger,
                                            MEMORY_MAP_WAV_MCU_INTF,
                                            messenger_send_reg_if,
                                            messenger_receive_reg_if,
                                            validate_message))
    {
        shutdown(0x0001);
    }

    messenger_init_reg_if(&messenger, MEMORY_MAP_WAV_MCU_INTF, 0x10, 0x11);

    message.id = 0x0000000F;
    message.data = 0xFFFFFFFF;
    xSendMessage(messenger.address, &message);

    // Receiving loop
    for (;;)
    {
        if (xReceiveMessage(messenger.address, &message))
        {
            // Special ID for ending the test
            if (message.id == 0xFFFFFFFF)
            {
                shutdown(message.data);
                break;
            }

            // Echo message
            xSendMessage(messenger.address, &message);
        }
    }
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
    shutdown(0x20001);
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
    shutdown(0x30001);
}

/*-----------------------------------------------------------*/
void vApplicationTickHook( void )
{
    /* The tests in the full demo expect some interaction with interrupts. */
}

/*-----------------------------------------------------------*/
void vAssertCalled( const char * const pcFileName, unsigned long ulLine )
{
    const char * const msg = "Assertion occured!\n";
    const char *pcString = pcFileName;
    uint32_t ulFileNameLen = strlen(pcFileName);
    char cFileName[7] = {'\0'};

    /**
     * @note    This is a patch because on this platform it is known that assert
     *          will fail for port.c line 161.
     */
    memcpy(&cFileName[0], &pcString[ulFileNameLen - 6], 6);

    if (strcmp(pcFileName, "port.c") && ulLine == 161)
    {
        return;
    }

    write( STDOUT_FILENO, msg, strlen( msg ) );
    write( STDOUT_FILENO, pcFileName, strlen( pcFileName ) );
    shutdown(0x40001);
}

/*-----------------------------------------------------------*/
static void shutdown(uint32_t cause)
{
    const char * const msg = "Shutting down!\n";
    taskDISABLE_INTERRUPTS();
    write( STDOUT_FILENO, msg, strlen( msg ) );
    _exit(1);
}
