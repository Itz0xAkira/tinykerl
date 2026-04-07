#pragma once
#include <stdint.h>

/** @brief VGA text buffer width in columns. */
#define VGA_WIDTH  80
/** @brief VGA text buffer height in rows. */
#define VGA_HEIGHT 25

/** @brief VGA hardware color indices for foreground and background. */
typedef enum {
    VGA_COLOR_BLACK         = 0,
    VGA_COLOR_BLUE          = 1,
    VGA_COLOR_GREEN         = 2,
    VGA_COLOR_CYAN          = 3,
    VGA_COLOR_RED           = 4,
    VGA_COLOR_MAGENTA       = 5,
    VGA_COLOR_BROWN         = 6,
    VGA_COLOR_LIGHT_GREY    = 7,
    VGA_COLOR_DARK_GREY     = 8,
    VGA_COLOR_LIGHT_BLUE    = 9,
    VGA_COLOR_LIGHT_GREEN   = 10,
    VGA_COLOR_LIGHT_CYAN    = 11,
    VGA_COLOR_LIGHT_RED     = 12,
    VGA_COLOR_LIGHT_MAGENTA = 13,
    VGA_COLOR_LIGHT_BROWN   = 14,
    VGA_COLOR_WHITE         = 15,
} vga_color_t;

/**
 * @brief Packs a foreground and background color into a single attribute byte.
 *
 * @param fg Foreground color.
 * @param bg Background color.
 * @return Combined color attribute byte.
 */
static inline uint8_t vga_color(vga_color_t fg, vga_color_t bg) {
    return fg | (bg << 4);
}

/**
 * @brief Packs a character and color attribute into a VGA buffer entry.
 *
 * @param c     Character to display.
 * @param color Color attribute byte from vga_color().
 * @return 16-bit value suitable for writing directly to the VGA buffer.
 */
static inline uint16_t vga_entry(char c, uint8_t color) {
    return (uint16_t)c | ((uint16_t)color << 8);
}
