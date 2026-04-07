#pragma once
#include <stdint.h>

/**
 * @brief Initializes the PIT (8253 timer) and registers its IRQ0 handler.
 *
 * Programs channel 0 of the PIT to fire at @p hz interrupts per second.
 * Each tick increments an internal counter used by pit_ticks() and pit_sleep().
 *
 * @param hz Desired tick frequency in Hz (e.g. 1000 for 1ms resolution).
 */
void pit_init(uint32_t hz);

/**
 * @brief Returns the number of ticks since pit_init() was called.
 *
 * @return Tick count. At 1000 Hz this is milliseconds since boot.
 */
uint32_t pit_ticks(void);

/**
 * @brief Busy-waits for at least @p ms milliseconds.
 *
 * Halts the CPU between ticks to avoid burning cycles.
 *
 * @param ms Milliseconds to sleep. Requires pit_init(1000).
 */
void pit_sleep(uint32_t ms);
