/****************************************************************************
*****************************************************************************
** Wavious LLC Proprietary
**
** Copyright (c) 2021 Wavious LLC. All rights reserved.
**
** All data and information contained in or disclosed by this document
** are confidential and proprietary information of Wavious LLC,
** and all rights therein are expressly reserved. By accepting this
** material, the recipient agrees that this material and the information
** contained therein are held in confidence and in trust and will not be
** used, copied, reproduced in whole or in part, nor its contents
** revealed in any manner to others without the express written
** permission of Wavious LLC.
*****************************************************************************
****************************************************************************/
#include <FreeRTOS.h>
#include <task.h>
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
    pxCompletion->ucDone = 0;
    taskEXIT_CRITICAL();
    return xReturn;
}

void vWaitForCompletion(Completion_t *pxCompletion)
{
    xWaitForCompletionTimeout(pxCompletion, portMAX_DELAY);
}
