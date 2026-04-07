#pragma once
#include <stdint.h>

/**
 * @brief CPU register state pushed by the ISR/IRQ stubs.
 *
 * Passed by pointer to every interrupt and exception handler so the handler
 * can inspect or modify the interrupted context.
 */
typedef struct __attribute__((packed)) {
    uint32_t ds;
    uint32_t edi, esi, ebp, esp, ebx, edx, ecx, eax;
    uint32_t int_no;   /**< Interrupt vector number. */
    uint32_t err_code; /**< CPU error code, or 0 if none. */
    uint32_t eip, cs, eflags, useresp, ss;
} registers_t;

/** @brief Function pointer type for ISR and IRQ handlers. */
typedef void (*isr_handler_t)(registers_t *);

/**
 * @brief Initializes the Interrupt Descriptor Table.
 *
 * Installs stubs for all 32 CPU exceptions and 16 PIC IRQs (remapped to
 * vectors 0x20-0x2F), then calls lidt to load the table.
 *
 * @note pic_init() must be called before idt_init() so IRQ vectors do not
 *       collide with CPU exceptions.
 */
void idt_init(void);

/**
 * @brief Registers a C handler for a given interrupt vector.
 *
 * @param n       Interrupt vector number (0-255).
 * @param handler Function to call when vector @p n fires.
 */
void idt_register_handler(uint8_t n, isr_handler_t handler);
