#ifndef _KERNEL_IRQ_H_
#define _KERNEL_IRQ_H_

#include <stdint.h>
#include <metal/interrupt.h>

/** @brief  Halt CPU until interrupt fires */
__attribute__((always_inline))
static inline void wait_for_interrupt(void)
{
    __asm__ volatile("wfi");
}

/** @brief  Disables all interrupts */
__attribute__((always_inline))
static inline void disable_interrupt_local(void)
{
    __asm__ volatile("csrc mstatus, 0x8");
}

/** @brief  Enables all interrupts */
__attribute__((always_inline))
static inline void enable_interrupt_local(void)
{
    __asm__ volatile("csrs mstatus, 0x8");
}

/** @brief  Disable individual interrupts */
__attribute__((always_inline))
static inline void interrupt_disable(uint32_t mask)
{
    __asm__ volatile("csrc mie, %0" : : "r" (mask));
}

/** @brief  Enable individual interrupts */
__attribute__((always_inline))
static inline void interrupt_enable(uint32_t mask)
{
    __asm__ volatile("csrs mie, %0" : : "r" (mask));
}

/** @brief  Disable individual interrupts and return previous state */
__attribute__((always_inline))
static inline uint32_t interrupt_disable_save(uint32_t mask, uint32_t flags)
{
    __asm__ volatile ("csrrc %0, mie, %1" : "=r" (flags) : "r" (mask));
    return flags;
}

/** @brief  Enable individual interrupts and return previous state */
__attribute__((always_inline))
static inline uint32_t interrupt_enable_save(uint32_t mask, uint32_t flags)
{
    __asm__ volatile ("csrrs %0, mie, %1" : "=r" (flags) : "r" (mask));
    return flags;
}
/** @brief  Restore individual interrupts to previous state */
__attribute__((always_inline))
static inline void interrupt_restore(uint32_t flags)
{
    __asm__ volatile("csrw mie, %0" : : "r" (flags));
}

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
irq_return_t request_irq(uint32_t irq, metal_interrupt_handler_t handler, void *args);

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
