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
