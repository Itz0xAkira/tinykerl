#pragma once
#include <stdint.h>

/**
 * @brief Initializes the COM1 serial port at 38400 baud.
 *
 * @return 1 on success, 0 if the loopback self-test fails.
 */
int serial_init(void);

/**
 * @brief Writes a single character to COM1.
 */
void serial_putchar(char c);

/**
 * @brief Writes a null-terminated string to COM1.
 */
void serial_print(const char *str);
