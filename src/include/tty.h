#pragma once
#include <stddef.h>
#include "vga.h"

/**
 * @brief Initializes the TTY and clears the screen.
 *
 * Resets the cursor to (0, 0), sets the default color to light grey on black,
 * and fills the entire VGA buffer with blank characters.
 */
void tty_init(void);

/**
 * @brief Sets the current text color for subsequent output.
 *
 * @param color Combined foreground/background byte produced by vga_color().
 */
void tty_setcolor(uint8_t color);

/**
 * @brief Writes a single character to the terminal at the current cursor.
 *
 * Handles newline (@c \\n) and advances the cursor, scrolling the screen
 * up one line when the bottom is reached.
 *
 * @param c Character to write.
 */
void tty_putchar(char c);

/**
 * @brief Writes @p len bytes from @p str to the terminal.
 *
 * @param str Pointer to the character data.
 * @param len Number of bytes to write.
 */
void tty_write(const char *str, size_t len);

/**
 * @brief Writes a null-terminated string to the terminal.
 *
 * @param str Null-terminated string to print.
 */
void tty_print(const char *str);
