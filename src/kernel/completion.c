/**
 * Copyright (c) 2021 Wavious LLC.
 *
 * SPDX-License-Identifier: Apache-2.0
 */

/* FreeRTOS includes. */
#include <FreeRTOS.h>
#include <task.h>

/* Kernel includes. */
#include <kernel/completion.h>

void vInitCompletion(Completion_t *pxCompletion)
{
    pxCompletion->xHandle = NULL;
    pxCompletion->ucDone = 0;
}

void vReInitCompletion(Completion_t *pxCompletion)
{
    pxCompletion->ucDone = 0;
}

void vComplete(Completion_t *pxCompletion)
{
    TaskHandle_t xHandle;
    taskENTER_CRITICAL();
    {
        pxCompletion->ucDone++;
        xHandle = pxCompletion->xHandle;
    }
    taskEXIT_CRITICAL();

    // Notify task that is waiting
    if (xHandle)
    {
        xTaskNotifyGive(xHandle);
    }
}

void vCompleteFromISR(Completion_t *pxCompletion, BaseType_t *pxHigherPriorityTaskWoken)
{
    BaseType_t xFlags;
    TaskHandle_t xHandle;
    xFlags = taskENTER_CRITICAL_FROM_ISR();
    {
        pxCompletion->ucDone++;
        xHandle = pxCompletion->xHandle;
    }
    taskEXIT_CRITICAL_FROM_ISR(xFlags);

    // Notify task that is waiting
    if (xHandle)
    {
        vTaskNotifyGiveFromISR(xHandle, pxHigherPriorityTaskWoken);
    }
}

BaseType_t xWaitForCompletionTimeout(Completion_t *pxCompletion, TickType_t xBlockTime)
{
    BaseType_t xReturn = pdTRUE;

    taskENTER_CRITICAL();
    if (!pxCompletion->ucDone)
    {
        pxCompletion->xHandle = xTaskGetCurrentTaskHandle();
        taskEXIT_CRITICAL();
        xReturn = ulTaskNotifyTake(pdTRUE, xBlockTime);
        taskENTER_CRITICAL();
    }
    pxCompletion->ucDone--;
    taskEXIT_CRITICAL();
    return xReturn;
}

void vWaitForCompletion(Completion_t *pxCompletion)
{
    xWaitForCompletionTimeout(pxCompletion, portMAX_DELAY);
}
