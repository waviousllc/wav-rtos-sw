/**
 * Copyright (c) 2021 Wavious LLC.
 *
 * SPDX-License-Identifier: Apache-2.0
 */
#ifndef _KERNEL_IRQ_H_
#define _KERNEL_IRQ_H_

/* Standard includes. */
#include <stdint.h>

/* Hardware Abstraction Layer include. */
#include <hal/irq.h>

//  Map directly to HAL implementation
#define wait_for_interrupt              hal_wait_for_interrupt
#define disable_interrupt_local         hal_disable_interrupt_local
#define enable_interrupt_local          hal_enable_interrupt_local
#define interrupt_disable               hal_interrupt_disable
#define interrupt_enable                hal_interrupt_enable
#define interrupt_disable_save          hal_interrupt_disable_save
#define interrupt_enable_save           hal_interrupt_enable_save
#define interrupt_restore               hal_interrupt_restore

// Prototype for generic IRQ handler function
typedef void (*irq_handler_t)(int, void *);

/**
 * @brief   IRQ Registration Return Enumerations
 *
 * @details Enumerations for return values from request_irq function.
 *
 * @IRQ_RETURN_REGISTERED           Successfully registered.
 * @IRQ_RETURN_INVALID_IRQ_NUM      IRQ number is out of range.
 * @IRQ_RETURN_ERROR                Generic error with regsitration.
 */
typedef enum irq_return_t
{
    IRQ_RETURN_REGISTERED,
    IRQ_RETURN_INVALID_IRQ_NUM,
    IRQ_RETURN_ERROR,
} irq_return_t;

/**
 * @brief   Register IRQ handler with Kernel IRQ subsystem
 *
 * @details Registers an IRQ handler with the given irq number and enables
 *          the IRQ.
 *
 * @param[in]   irq     number of IRQ that handler belongs to.
 * @param[in]   handler pointer to handler function.
 * @param[in]   args    callback data.
 *
 * @return      returns whether registration was successful.
 *
 * @retval      IRQ_REGISTERED                   succesfully regsitered.
 * @retval      IRQ_RETURN_INVALID_IRQ_NUM       IRQ is out of range.
 * @retval      IRQ_RETURN_ERROR                 otherwise.
 */
irq_return_t request_irq(uint32_t irq, irq_handler_t handler, void *args);

/**
 * @brief   Free IRQ
 *
 * @details Unregisters an IRQ Handler with given irq number and disables the
 *          IRQ.
 *
 * @param[in]   irq     number of IRQ to free.
 *
 * @retval      IRQ_REGISTERED                   succesfully regsitered.
 * @retval      IRQ_RETURN_INVALID_IRQ_NUM       IRQ is out of range.
 * @retval      IRQ_RETURN_ERROR                 otherwise.
 */
irq_return_t free_irq(uint32_t irq);

/**
 * @brief   Enable IRQ in Kernel subsystem
 *
 * @details Enables the given IRQ in the Kernel. If this IRQ is enabled, but no
 *          handler is registered, then this function returns a negative value.
 *
 * @param[in]   irq     which IRQ to enable.
 *
 * @return      returns whether IRQ was enabled.
 *
 * @retval      0   if enabled.
 * @retval     -1   if not enabled, could be out of bounds IRQ number or
 *                  IRQ doesn't have a handler registered.
 */
int8_t enable_irq(uint32_t irq);

/**
 * @brief   Disable IRQ in Kernel subsystem
 *
 * @brief   Disables the given IRQ in the Kernel. It is okay to call this
 *          function for an IRQ that is not registered.
 *
 * @param[in]   irq     which IRQ to disable.
 *
 * @retval      0   if disabled.
 * @retval     -1   if not disabled, could be out of bounds IRQ number.
 */
int8_t disable_irq(uint32_t irq);

#endif /* _KERNEL_IRQ_H_ */
