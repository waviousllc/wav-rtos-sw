/* Copyright 2019 SiFive, Inc */
/* SPDX-License-Identifier: Apache-2.0 */
/* Modified by waviousllc */

/* FreeRTOS kernel includes. */
#include <FreeRTOS.h>
#include <task.h>

/* Freedom metal includes. */
#include <metal/machine.h>
#include <metal/machine/platform.h>

#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include <board/board.h>

#define TIMER_INC_ONE_TICK  ((size_t) ( ( configCPU_CLOCK_HZ ) / ( configTICK_RATE_HZ ) ))

__attribute__ ((aligned(4))) uint8_t ucHeap[ configTOTAL_HEAP_SIZE ] __attribute__ ((section (".heap")));

__attribute__((constructor)) static void FreeRTOS_init(void);

#ifndef WAV_MCU_IRQ_SYNC_CFG
    // Error out compilation
    #error WAV_MCU_IRQ_SYNC_CFG must be defined
    // Never used but defining so warnings below aren't generated
    #define WAV_MCU_IRQ_SYNC_CFG (0x00000000)
#endif
#ifndef WAV_MCU_IRQ_EDGE_CFG
    // Error out compilation
    #error WAV_MCU_IRQ_EDGE_CFG must be defined
    // Never used but defining so warnings below aren't generated
    #define WAV_MCU_IRQ_EDGE_CFG (0x00000000)
#endif

/**
 * Override weak declarations as not needed for our application.
 */
void metal_init_run(void)
{
    // Intentionally empty
}

void metal_fini_run(void)
{
    // Intentionally empty
}

#if ( configUSE_TIMERS == 1 )
static uint64_t ullNextTime = 0ULL;
static volatile uint64_t *pullMachineTimerCompareRegister;
static volatile uint32_t *pullMachineTimerCompareLoadRegister;
void vPortSetupTimerInterrupt( void )
{
    uint32_t ulCurrentTimeHigh, ulCurrentTimeLow;

    volatile uint32_t * const pulTimeHigh = ( volatile uint32_t * const ) ( MEMORY_MAP_WAV_MCU + WAV_MCU_MTIME_HI_STA__ADR );
    volatile uint32_t * const pulTimeLow = ( volatile uint32_t * const ) ( MEMORY_MAP_WAV_MCU + WAV_MCU_MTIME_LO_STA__ADR );
    volatile uint32_t ulHartId;

    __asm volatile( "csrr %0, mhartid" : "=r"( ulHartId ) );
    pullMachineTimerCompareRegister  = ( volatile uint64_t * ) ( MEMORY_MAP_WAV_MCU + WAV_MCU_MTIMECMP_LO_CFG__ADR );
    pullMachineTimerCompareLoadRegister = ( volatile uint32_t *) ( MEMORY_MAP_WAV_MCU + WAV_MCU_MTIMECMP_CFG__ADR );

    do
    {
        ulCurrentTimeHigh = *pulTimeHigh;
        ulCurrentTimeLow = *pulTimeLow;
    } while( ulCurrentTimeHigh != *pulTimeHigh );

    ullNextTime = ( uint64_t ) ulCurrentTimeHigh;
    ullNextTime <<= 32ULL; /* High 4-byte word is 32-bits up. */
    ullNextTime |= ( uint64_t ) ulCurrentTimeLow;
    ullNextTime += ( uint64_t ) TIMER_INC_ONE_TICK;
    *pullMachineTimerCompareRegister = ullNextTime;
    *pullMachineTimerCompareLoadRegister = 0x1;

    // For handler
    ullNextTime += ( uint64_t ) TIMER_INC_ONE_TICK;

    // Turn on timer interrupt
    __asm__ volatile("csrs mie, %0" :: "r"(0x80));
}
#endif /* configUSE_TIMERS == 1 */

__attribute__((constructor)) static void FreeRTOS_init(void)
{
    struct metal_cpu *cpu;
    struct metal_interrupt *cpu_intr;
    extern BaseType_t xPortFreeRTOSInit( StackType_t xIsrStack );
    volatile uint32_t * const wavMcuIrqEdgeCfg = (( volatile uint32_t * const ) ( MEMORY_MAP_WAV_MCU + WAV_MCU_IRQ_FAST_EDGE_CFG__ADR ));
    volatile uint32_t * const wavMcuIrqSyncCfg = (( volatile uint32_t * const ) ( MEMORY_MAP_WAV_MCU + WAV_MCU_IRQ_FAST_SYNC_CFG__ADR ));

    const char * const pcErrorMsg = "No External controller\n";

    /* Remove compiler warning about unused parameter. */
    ( void ) pcErrorMsg;

    *wavMcuIrqEdgeCfg = WAV_MCU_IRQ_EDGE_CFG;
    *wavMcuIrqSyncCfg = WAV_MCU_IRQ_SYNC_CFG;

    // Initialize Metal interrupt controller
    cpu = metal_cpu_get(metal_cpu_get_current_hartid());
    if (cpu == NULL)
    {
        return;
    }

    cpu_intr = metal_cpu_interrupt_controller(cpu);
    if (cpu_intr == NULL)
    {
        return;
    }
    metal_interrupt_init(cpu_intr);

    // Set mtvec to custom interrupt vector
    __asm__ __volatile__ (
        "la t0, __metal_vector_table\n"
        "csrw mtvec, t0"
    );
}

void FreedomMetal_InterruptHandler( uint32_t cause )
{
    int id;
    void *priv;
    struct __metal_driver_riscv_cpu_min_intc *intc;
    struct metal_cpu *cpu;

    // Always HART_ID 0
    cpu = metal_cpu_get(0);

    if ( cpu ) {

        intc = (struct __metal_driver_riscv_cpu_min_intc *) metal_cpu_interrupt_controller(cpu);
        id = cause & METAL_MCAUSE_CAUSE;

    #if ( configUSE_TIMERS == 1 )
        if (id == METAL_INTERRUPT_ID_TMR)
        {
            *pullMachineTimerCompareRegister = ullNextTime;
            *pullMachineTimerCompareLoadRegister = 0x1;
            ullNextTime += ( uint64_t ) TIMER_INC_ONE_TICK;
            if (xTaskIncrementTick())
            {
                vTaskSwitchContext();
            }
            return;
        }
    #endif /* configUSE_TIMERS == 1 */

        /**
         * Local and External Interrupts call into metal where user
         * registered ISR for each of these interrupts. Regardless
         * of vectored or direct mode call implementation.
         */
        if ((id >= METAL_INTERRUPT_ID_LC0) || (id == METAL_INTERRUPT_ID_EXT))
        {
            priv = intc->metal_int_table[id].exint_data;
            if (intc->metal_int_table[id].handler != NULL)
                intc->metal_int_table[id].handler(id, priv);
            goto cleanup;
        }
    }

cleanup:
    return;
}
