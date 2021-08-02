
/**
 * Copyright (c) 2021 Wavious LLC.
 *
 * SPDX-License-Identifier: Apache-2.0
 */
#ifndef _FREERTOS_CONFIG_H_
#define _FREERTOS_CONFIG_H_

/* Here is a good place to include header files that are required across
your application. */
#define configMTIMECMP_BASE_ADDRESS             0
#define configMTIME_BASE_ADDRESS                0
#define configUSE_PREEMPTION                    1
#define configUSE_PORT_OPTIMISED_TASK_SELECTION 1
#define configUSE_TICKLESS_IDLE                 0
#define configCPU_CLOCK_HZ                      38400000
#define configTICK_RATE_HZ                      ( ( TickType_t ) 1000)
#define configMAX_PRIORITIES                    7
#define configMINIMAL_STACK_SIZE                ( ( size_t ) 256 )
#define configMAX_TASK_NAME_LEN                 16
#define configUSE_16_BIT_TICKS                  0
#define configIDLE_SHOULD_YIELD                 0
#define configUSE_TASK_NOTIFICATIONS            1
#define configTASK_NOTIFICATION_ARRAY_ENTRIES   1
#define configUSE_MUTEXES                       0
#define configUSE_RECURSIVE_MUTEXES             0
#define configUSE_COUNTING_SEMAPHORES           1
#define configQUEUE_REGISTRY_SIZE               8
#define configUSE_TIME_SLICING                  0
#define configUSE_NEWLIB_REENTRANT              1
#define configUSE_APPLICATION_TASK_TAG          0

#define configSTACK_DEPTH_TYPE                  uint16_t

/* Memory allocation related definitions. */
#define configSUPPORT_STATIC_ALLOCATION             0
#define configSUPPORT_DYNAMIC_ALLOCATION            1
#define configTOTAL_HEAP_SIZE                       ( ( size_t ) 4096 * sizeof( size_t ) )  // 16 KB
#define configAPPLICATION_ALLOCATED_HEAP            1

/* Hook function related definitions. */
#define configUSE_IDLE_HOOK                     1
#define configUSE_TICK_HOOK                     0
#define configCHECK_FOR_STACK_OVERFLOW          1
#define configUSE_MALLOC_FAILED_HOOK            1
#define configUSE_DAEMON_TASK_STARTUP_HOOK      0

/* Run time and task stats gathering related definitions. */
#define configGENERATE_RUN_TIME_STATS           0
#define configUSE_TRACE_FACILITY                0
#define configUSE_STATS_FORMATTING_FUNCTIONS    0

/* Co-routine related definitions. */
#define configUSE_CO_ROUTINES                   0
#define configMAX_CO_ROUTINE_PRIORITIES         1

/* Software timer related definitions. */
#define configUSE_TIMERS                        1
#define configTIMER_TASK_PRIORITY               1
#define configTIMER_QUEUE_LENGTH                8
#define configTIMER_TASK_STACK_DEPTH            configMINIMAL_STACK_SIZE

/* Define to trap errors during development. */
#ifdef DEBUG
    void vAssertCalled( const char * const pcFileName, unsigned long ulLine );
    #define configASSERT(x) if( ( x ) == 0 ) vAssertCalled( __FILE__, __LINE__ )
#endif /* DEBUG */

/* Optional functions - most linkers will remove unused functions anyway. */
#define INCLUDE_vTaskPrioritySet                1
#define INCLUDE_uxTaskPriorityGet               0
#define INCLUDE_vTaskDelete                     1
#define INCLUDE_vTaskSuspend                    0
#define INCLUDE_xResumeFromISR                  0
#define INCLUDE_vTaskDelayUntil                 1
#define INCLUDE_vTaskDelay                      1
#define INCLUDE_xTaskGetSchedulerState          0
#define INCLUDE_xTaskGetCurrentTaskHandle       1
#define INCLUDE_uxTaskGetStackHighWaterMark     0
#define INCLUDE_xTaskGetIdleTaskHandle          0
#define INCLUDE_eTaskGetState                   0
#define INCLUDE_xEventGroupSetBitFromISR        0
#define INCLUDE_xTimerPendFunctionCall          0
#define INCLUDE_xTaskAbortDelay                 0
#define INCLUDE_xTaskGetHandle                  0
#define INCLUDE_xTaskResumeFromISR              0

/*******************************************************************************
**                          UNUSED DEFINITIONS
*******************************************************************************/
// #define configSYSTICK_CLOCK_HZ                  1000000
// #define configUSE_QUEUE_SETS                    0
// #define configENABLE_BACKWARD_COMPATIBILITY     0
// #define configNUM_THREAD_LOCAL_STORAGE_POINTERS 5
// #define configMESSAGE_BUFFER_LENGTH_TYPE        size_t
// #define configSTACK_ALLOCATION_FROM_SEPARATE_HEAP   1

#endif /* _FREERTOS_CONFIG_H_ */
