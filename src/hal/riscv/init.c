/**
 * Copyright (c) 2021 Wavious LLC.
 *
 * SPDX-License-Identifier: Apache-2.0
 */

/* Standard includes. */
#include <stdio.h>
#include <string.h>
#include <unistd.h>

/* Board includes. */
#include <board/board.h>
#include <board/memory_map.h>

/* Freedom metal includes. */
#include <metal/machine.h>
#include <metal/machine/platform.h>

/* OSAL includes. */
#include <osal/time.h>

static uint64_t _prv_timer_rate   = 0UL;
static uint64_t ullNextTime       = 0ULL;
static volatile uint64_t *pullMachineTimerCompareRegister;

void hal_timer_init(uint64_t timer_rate)
{
    uint32_t ulCurrentTimeHigh, ulCurrentTimeLow;

    volatile uint32_t * const pulTimeHigh = ( volatile uint32_t * const ) MTIME_BASE_ADDRESS_HI;
    volatile uint32_t * const pulTimeLow = ( volatile uint32_t * const ) MTIME_BASE_ADDRESS_LO;
    volatile uint32_t ulHartId;

    // Initialize the timer rate
    _prv_timer_rate = timer_rate;

    __asm volatile( "csrr %0, mhartid" : "=r"( ulHartId ) );
    pullMachineTimerCompareRegister  = ( volatile uint64_t * ) MTIME_CMP_ADDRESS(ulHartId);

    do
    {
        ulCurrentTimeHigh = *pulTimeHigh;
        ulCurrentTimeLow = *pulTimeLow;
    } while( ulCurrentTimeHigh != *pulTimeHigh );

    ullNextTime = ( uint64_t ) ulCurrentTimeHigh;
    ullNextTime <<= 32ULL; /* High 4-byte word is 32-bits up. */
    ullNextTime |= ( uint64_t ) ulCurrentTimeLow;
    ullNextTime += ( uint64_t ) _prv_timer_rate;
    *pullMachineTimerCompareRegister = ullNextTime;
    // Board specific reload procedure
    MTIME_RELOAD();

    // For handler
    ullNextTime += ( uint64_t ) _prv_timer_rate;

    // Turn on timer interrupt
    __asm__ volatile("csrs mie, %0" :: "r"(0x80));
}

void hal_subsystem_init(void)
{
    struct metal_cpu *cpu;
    struct metal_interrupt *cpu_intr;
    const char * const pcErrorMsg = "No External controller\n";

    /* Remove compiler warning about unused parameter. */
    ( void ) pcErrorMsg;

    // Initialize the board specific code
    board_init();

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
            _exit(-1);
        }
        metal_interrupt_init(plic);
    }
#endif
}

void hal_handle_interrupt(uint32_t cause)
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

        if (id == METAL_INTERRUPT_ID_TMR)
        {
            *pullMachineTimerCompareRegister = ullNextTime;
            MTIME_RELOAD();
            ullNextTime += ( uint64_t ) _prv_timer_rate;

            // Let OS know that a tick occurred
            osal_tick_periodic();
            return;
        }

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
        }
    }
}
