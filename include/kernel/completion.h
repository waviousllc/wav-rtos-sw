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
#ifndef _KERNEL_COMPLETION_H_
#define _KERNEL_COMPLETION_H_

#include <FreeRTOS.h>
#include <task.h>
#include <metal/lock.h>

/**
 * @brief   Completion Structure
 *
 * @details Completion structure that can be used to synchronize a task with the
 *          completion of an event. NOTE: Only one task can be sychronized at a
 *          given time.
 *
 * @ucDone      done flag to indicate that event occurred.
 * @xTaskHandle pointer to the current task that is waiting on the completion.
 * @lock        spinlock for locking completion state.
 */
typedef struct completion
{
    uint8_t         ucDone;
    TaskHandle_t    xHandle;
    struct metal_lock lock;
} Completion_t;

/**
 * @brief   Initialize Completion
 *
 * @details Initalizes a completion variable. Must be called prior to using
 *          a completion variable.
 *
 * @param[in]   pxCompletion    pointer to completion variable.
 *
 * @return      void.
 */
void vInitCompletion(Completion_t *pxCompletion);

/**
 * @brief   Reinitializes Completion
 *
 * @details Reinitializes a completion variable. Should be called if a
 *          completion variable is reused.
 *
 * @param[in]   pxCompletion    pointer to completion variable.
 *
 * @return      void.
 */
void vReInitCompletion(Completion_t *pxCompletion);

/**
 * @brief   Wait For Completion Timeout
 *
 * @details Waits for the completion variable to complete or for the timeout
 *          to occur.
 *
 * @param[in]   pxCompletion    pointer to completion variable.
 *
 * @return      returns whether completion finished or timed out.
 * @retval      pdTRUE if completion finished.
 * @retval      pdFALSE if completion timed out.
 */
BaseType_t xWaitForCompletionTimeout(Completion_t *pxCompletion, TickType_t xBlockTime);

/**
 * @brief   Wait For Completion Timeout
 *
 * @details Waits for the completion variable to complete indefinitely.
 *
 * @param[in]   pxCompletion    pointer to completion variable.
 *
 * @return      void.
 */
void vWaitForCompletion(Completion_t *pxCompletion);

/**
 * @brief   Complete
 *
 * @details Marks completion variable as complete. Wakes up any task waiting
 *          for completion.
 *
 * @param[in]   pxCompletion    pointer to completion variable.
 *
 * @return      void.
 */
void vComplete(Completion_t *pxCompletion);

/**
 * @brief   Complete From ISR
 *
 * @details ISR version of vComplete. Must call this function if executing from
 *          an ISR.
 *
 * @param[in]   pxCompletion                pointer to completion variable.
 * @param[out]  pxHigherPriorityTaskWoken   pointer to flag to indicate if context
 *                                          switch should occur.
 *
 * @return      void.
 */
void vCompleteFromISR(Completion_t *pxCompletion, BaseType_t *pxHigherPriorityTaskWoken);

#endif /* _KERNEL_COMPLETION_H_ */
