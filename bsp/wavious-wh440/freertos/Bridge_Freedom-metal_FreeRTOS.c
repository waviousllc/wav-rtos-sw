/* Copyright 2019 SiFive, Inc */
/* SPDX-License-Identifier: Apache-2.0 */

/* FreeRTOS kernel includes. */
#include <FreeRTOS.h>
#include <task.h>

/* Freedom metal includes. */
#include <metal/machine.h>
#include <metal/machine/platform.h>

#include <stdio.h>
#include <string.h>
#include <unistd.h>

#if( configUSE_SEGGER_SYSTEMVIEW == 1 )
# include "SEGGER_SYSVIEW_FreeRTOS.h"
#endif

#define        TIMER_INC_ONE_TICK      (( size_t ) ( ( configCPU_CLOCK_HZ ) / ( configTICK_RATE_HZ ) ))

static uint64_t ullNextTime = 0ULL;
static volatile uint64_t *pullMachineTimerCompareRegister;

__attribute__ ((aligned(4))) uint8_t ucHeap[ configTOTAL_HEAP_SIZE ] __attribute__ ((section (".heap")));

__attribute__((constructor)) static void FreeRTOS_init(void);
#ifdef SEGGER_SYSTEMVIEW
__attribute__((constructor)) static void SEGGER_SysView_init(void);
#endif

void vPortSetupTimerInterrupt( void )
{
       uint32_t ulCurrentTimeHigh, ulCurrentTimeLow;

       volatile uint32_t * const pulTimeHigh = ( volatile uint32_t * const ) ( ( MTIME_CTRL_ADDR + 0xBFF8 ) + 4UL ); /* 8-byte typer so high 32-bit word is 4 bytes up. */
       volatile uint32_t * const pulTimeLow = ( volatile uint32_t * const ) ( MTIME_CTRL_ADDR + 0xBFF8 );
       volatile uint32_t ulHartId;

       __asm volatile( "csrr %0, mhartid" : "=r"( ulHartId ) );
       pullMachineTimerCompareRegister  = ( volatile uint64_t * ) ( MTIME_CTRL_ADDR + 0x4000 + ( ulHartId * sizeof( uint64_t ) ) );

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


__attribute__((constructor)) static void FreeRTOS_init(void)
{
    struct metal_cpu *cpu;
    struct metal_interrupt *cpu_intr;
    extern BaseType_t xPortFreeRTOSInit( StackType_t xIsrStack );

    const char * const pcErrorMsg = "No External controller\n";

    /* Remove compiler warning about unused parameter. */
    ( void ) pcErrorMsg;

    /*
     * Initilize freedom-metal interrupt managment.
     *   Its SHOULD be made before calling xPortFreeRTOSInit because
     *   the interrupt/exeception handler MUST be the freertos handler.
     */
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
#endif

#ifdef METAL_SIFIVE_CLIC0
    {
        struct metal_interrupt *clic;

        // Check we this target has a plic. If not gracefull exit
        clic = metal_interrupt_get_controller(METAL_CLIC_CONTROLLER, 0);
        if (clic == NULL) {
            write( STDOUT_FILENO, pcErrorMsg, strlen( pcErrorMsg ) );

            for( ;; );
        }
        metal_interrupt_init(clic);
    }
#endif

    // enable timer interrupt
    // mie
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
    struct __metal_driver_cpu *cpu;
    portUBASE_TYPE hartid;

    __asm__ __volatile__ ("csrr %0, mhartid" : "=r"(hartid));

    cpu = __metal_cpu_table[hartid];

    if ( cpu ) {

        intc = (struct __metal_driver_riscv_cpu_intc *)
          __metal_driver_cpu_interrupt_controller((struct metal_cpu *)cpu);
        id = cause & METAL_MCAUSE_CAUSE;

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

void FreedomMetal_ExceptionHandler( void )
{
    int id;
    uintptr_t mcause, hartid;
    struct __metal_driver_riscv_cpu_intc *intc;
    struct __metal_driver_cpu *cpu;

    __asm__ __volatile__ ("csrr %0, mhartid" : "=r"(hartid));
    cpu = __metal_cpu_table[hartid];

    if ( cpu ) {
        intc = (struct __metal_driver_riscv_cpu_intc *)
          __metal_driver_cpu_interrupt_controller((struct metal_cpu *)cpu);

        __asm__ __volatile__ ("csrr %0, mcause" : "=r"(mcause));
        id = mcause & METAL_MCAUSE_CAUSE;

        configASSERT( id < METAL_ECALL_U_EXCEPTION_CODE );

        if (id < METAL_ECALL_U_EXCEPTION_CODE) {
            if (intc->metal_exception_table[id] != NULL)
                intc->metal_exception_table[id]((struct metal_cpu *)cpu, id);
        }
    }

    for( ;; ); // return is dangerous, we just got a critical exception.
    return;
}

#if( configUSE_SEGGER_SYSTEMVIEW == 1 )
__attribute__((constructor)) static void SEGGER_SysView_init(void)
{
    SEGGER_SYSVIEW_Conf();
  SEGGER_SYSVIEW_Start();
}

U32 SEGGER_SYSVIEW_X_GetInterruptId(void) {
#if (__riscv_xlen == 64)
  uintptr_t mcause;

  __asm__ __volatile__ ("csrr %0, mcause" : "=r"(mcause));

  if (mcause & 0x8000000000000000)
    mcause = mcause & 0x7FFFFFFFFFFFFFFF;
  else
    mcause = mcause & 0x8000000000000000;

  return (U32)mcause;
#elif (__riscv_xlen == 32)
  uintptr_t mcause;

  __asm__ __volatile__ ("csrr %0, mcause" : "=r"(mcause));

  if (mcause & 0x80000000)
    mcause = mcause & 0x7FFFFFFF;
  else
    mcause = mcause & 0x80000000;

  return (U32)mcause;
#endif
}

#ifndef configCLINT_BASE_ADDRESS
  #error No CLINT Base Address defined
#endif

U32 SEGGER_SYSVIEW_X_GetTimestamp(void) {
#if (__riscv_xlen == 64)
  return (U32)(*(( uint64_t * volatile ) ( configCLINT_BASE_ADDRESS + 0xBFF8) ));
#elif (__riscv_xlen == 32)
  uint32_t lo, hi;

    /* Guard against rollover when reading */
    do {
        hi = *(( uint32_t * volatile ) ( configCLINT_BASE_ADDRESS + 0xBFFC) );
        lo = *(( uint32_t * volatile ) ( configCLINT_BASE_ADDRESS + 0xBFF8) );
    } while ( *(( uint32_t * volatile ) ( configCLINT_BASE_ADDRESS + 0xBFFC)) != hi);

    return (U32)lo;
#endif
}

#endif
