#pragma once
#include <stdarg.h>

/**
 * @brief Formatted print to both the TTY and the serial port.
 *
 * Supports: %c, %s, %d (signed), %u (unsigned), %x (hex lowercase).
 *
 * @param fmt Format string.
 * @param ... Format arguments.
 */
void kprintf(const char *fmt, ...);

/**
 * @brief va_list variant of kprintf.
 *
 * @param fmt Format string.
 * @param args Argument list.
 */
void kvprintf(const char *fmt, va_list args);

/**
 * @brief Prints a formatted panic message and halts the system.
 *
 * Disables interrupts, prints the message prefixed with "PANIC: ",
 * then halts forever. Never returns.
 *
 * @param fmt Format string (same specifiers as kprintf).
 * @param ... Format arguments.
 */
__attribute__((noreturn)) void kpanic(const char *fmt, ...);
