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
#include <metal/lock.h>

void vInitCompletion(Completion_t *pxCompletion)
{
    pxCompletion->xHandle = NULL;
    pxCompletion->ucDone = 0;
    metal_lock_init(&pxCompletion->lock);
}

void vReInitCompletion(Completion_t *pxCompletion)
{
    pxCompletion->ucDone = 0;
}

void vComplete(Completion_t *pxCompletion)
{
    metal_lock_take(&pxCompletion->lock);
    pxCompletion->ucDone++;
    // Notify task that is waiting
    if (pxCompletion->xHandle)
    {
        xTaskNotifyGive(pxCompletion->xHandle);
    }
    metal_lock_give(&pxCompletion->lock);
}

void vCompleteFromISR(Completion_t *pxCompletion, BaseType_t *pxHigherPriorityTaskWoken)
{
    metal_lock_take(&pxCompletion->lock);
    pxCompletion->ucDone++;
    // Notify task that is waiting
    if (pxCompletion->xHandle)
    {
        vTaskNotifyGiveFromISR(pxCompletion->xHandle, pxHigherPriorityTaskWoken);
    }
    metal_lock_give(&pxCompletion->lock);
}

BaseType_t xWaitForCompletionTimeout(Completion_t *pxCompletion, TickType_t xBlockTime)
{
    BaseType_t xReturn = pdTRUE;

    metal_lock_take(&pxCompletion->lock);
    if (!pxCompletion->ucDone)
    {
        metal_lock_give(&pxCompletion->lock);
        xReturn = ulTaskNotifyTake(pdTRUE, xBlockTime);
        metal_lock_take(&pxCompletion->lock);
    }
    pxCompletion->ucDone = 0;
    metal_lock_give(&pxCompletion->lock);
    return xReturn;
}

void vWaitForCompletion(Completion_t *pxCompletion)
{
    xWaitForCompletionTimeout(pxCompletion, portMAX_DELAY);
}
