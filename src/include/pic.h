#pragma once
#include <stdint.h>

/**
 * @brief Initializes and remaps the 8259 PIC.
 *
 * Remaps IRQ0-7 to vectors 0x20-0x27 and IRQ8-15 to 0x28-0x2F so they do
 * not collide with CPU exception vectors 0-31. All IRQ lines are unmasked
 * after initialization.
 */
void pic_init(void);

/**
 * @brief Sends an End-Of-Interrupt signal to the PIC.
 *
 * Must be called at the end of every IRQ handler or the PIC will not fire
 * further interrupts on that line.
 *
 * @param irq IRQ line number (0-15).
 */
void pic_eoi(uint8_t irq);

/**
 * @brief Masks (disables) an IRQ line.
 *
 * @param irq IRQ line number (0-15).
 */
void pic_mask(uint8_t irq);

/**
 * @brief Unmasks (enables) an IRQ line.
 *
 * @param irq IRQ line number (0-15).
 */
void pic_unmask(uint8_t irq);
