#pragma once
#include <stdint.h>
#include <stdbool.h>

/**
 * @brief Initializes the PS/2 keyboard driver.
 *
 * Registers an IRQ1 handler that reads scancodes from port 0x60, translates
 * them to ASCII (with shift support), and stores them in a circular buffer.
 */
void keyboard_init(void);

/**
 * @brief Returns true if there is a character waiting in the input buffer.
 *
 * @return true if keyboard_getchar() will return immediately, false otherwise.
 */
bool keyboard_haschar(void);

/**
 * @brief Reads the next character from the keyboard input buffer.
 *
 * Blocks by halting the CPU until a key is available.
 *
 * @return ASCII character of the pressed key.
 */
char keyboard_getchar(void);
