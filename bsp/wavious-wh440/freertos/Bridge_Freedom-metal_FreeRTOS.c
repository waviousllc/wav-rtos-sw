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

#define MTIME_BASE_ADDRESS      ( ( MEMORY_MAP_CLINT ) + 0xBFF8UL )
#define MTIMECMP_BASE_ADDRESS   ( ( MEMORY_MAP_CLINT ) + 0x4000UL )

#define TIMER_INC_ONE_TICK  ((size_t) ( ( configCPU_CLOCK_HZ ) / ( configTICK_RATE_HZ ) ))

__attribute__ ((aligned(4))) uint8_t ucHeap[ configTOTAL_HEAP_SIZE ] __attribute__ ((section (".heap")));

__attribute__((constructor)) static void FreeRTOS_init(void);

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
void vPortSetupTimerInterrupt( void )
{
       uint32_t ulCurrentTimeHigh, ulCurrentTimeLow;

       volatile uint32_t * const pulTimeHigh = ( volatile uint32_t * const ) ( ( MTIME_BASE_ADDRESS ) + 4UL );
       volatile uint32_t * const pulTimeLow = ( volatile uint32_t * const ) ( MTIME_BASE_ADDRESS );
       volatile uint32_t ulHartId;

       __asm volatile( "csrr %0, mhartid" : "=r"( ulHartId ) );
       pullMachineTimerCompareRegister  = ( volatile uint64_t * ) ( MTIMECMP_BASE_ADDRESS + ( ulHartId * sizeof( uint64_t ) ) );

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

    const char * const pcErrorMsg = "No External controller\n";

    /* Remove compiler warning about unused parameter. */
    ( void ) pcErrorMsg;

    // Initialize Metal interrupt controllers
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

#ifdef METAL_RISCV_PLIC0
    {
        struct metal_interrupt *plic;

        // Check if this target has a plic. If not gracefull exit
        plic = metal_interrupt_get_controller(METAL_PLIC_CONTROLLER, 0);
        if (plic == NULL) {
            write( STDOUT_FILENO, pcErrorMsg, strlen( pcErrorMsg ) );

            for( ;; );
        }
        metal_interrupt_init(plic);
    }
#else
    #error METAL_RISCV_PLIC0 must be defined for Wavious-WH440 board.
#endif

    // Setup MTVEC
    __asm__ __volatile__ (
            "la t0, freertos_risc_v_trap_handler\n"
            "csrw mtvec, t0"
    );
}

void FreedomMetal_InterruptHandler( uint32_t cause )
{
    int id;
    void *priv;
    struct __metal_driver_riscv_cpu_intc *intc;
    struct metal_cpu *cpu;
    portUBASE_TYPE hartid;

    __asm__ __volatile__ ("csrr %0, mhartid" : "=r"(hartid));

    cpu = metal_cpu_get(metal_cpu_get_current_hartid());

    if ( cpu ) {

        intc = (struct __metal_driver_riscv_cpu_intc *)
          __metal_driver_cpu_interrupt_controller(cpu);
        id = cause & METAL_MCAUSE_CAUSE;

    #if ( configUSE_TIMERS == 1 )
        if (id == METAL_INTERRUPT_ID_TMR)
        {
            *pullMachineTimerCompareRegister = ullNextTime;
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
        {   priv = intc->metal_int_table[id].exint_data;
            if (intc->metal_int_table[id].handler != NULL)
                intc->metal_int_table[id].handler(id, priv);
        }
    }
}

// Defined in printf.c
void _putchar(char character)
{
    write(STDOUT_FILENO, &character, 1);
}
