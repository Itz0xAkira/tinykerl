#pragma once
#include <stdint.h>
#include <stdbool.h>

/**
 * @brief Serial port error flags.
 */
typedef enum {
    SERIAL_ERR_NONE    = 0,
    SERIAL_ERR_OVERRUN = (1 << 1),
    SERIAL_ERR_PARITY  = (1 << 2),
    SERIAL_ERR_FRAMING = (1 << 3),
    SERIAL_ERR_BREAK   = (1 << 4),
} serial_error_t;

/**
 * @brief Initializes the COM1 serial port with configurable baud rate.
 *
 * Enables IRQ4 for receive interrupts and buffers incoming data.
 *
 * @param baud Baud rate (e.g. 9600, 38400, 115200).
 * @return 1 on success, 0 if the loopback self-test fails (no serial hardware).
 */
int serial_init(uint32_t baud);

/**
 * @brief Writes a single character to COM1.
 *
 * @param c Character to transmit.
 */
void serial_putchar(char c);

/**
 * @brief Writes a null-terminated string to COM1.
 *
 * @param str String to transmit.
 */
void serial_print(const char *str);

/**
 * @brief Returns true if there is a character waiting in the receive buffer.
 *
 * @return true if serial_getchar() will return immediately, false otherwise.
 */
bool serial_haschar(void);

/**
 * @brief Reads the next character from the serial receive buffer.
 *
 * Blocks by halting the CPU until a character is available.
 *
 * @return Character received from COM1.
 */
char serial_getchar(void);

/**
 * @brief Returns the last error flags from the line status register.
 *
 * Clears the error state after reading.
 *
 * @return Bitwise OR of serial_error_t flags.
 */
uint8_t serial_get_error(void);
