#include <stddef.h>
#include "include/tty.h"
#include "include/vga.h"
#include "include/mem.h"
#include "include/io.h"

#define VGA_CTRL 0x3D4
#define VGA_DATA 0x3D5

static uint16_t *const VGA_BUF = (uint16_t *)0xB8000;
static size_t   tty_row;
static size_t   tty_col;
static uint8_t  tty_color;

static void cursor_update(void) {
    uint16_t pos = tty_row * VGA_WIDTH + tty_col;
    outb(VGA_CTRL, 0x0F); outb(VGA_DATA, pos & 0xFF);
    outb(VGA_CTRL, 0x0E); outb(VGA_DATA, (pos >> 8) & 0xFF);
}

static void tty_scroll(void) {
    for (size_t row = 1; row < VGA_HEIGHT; row++)
        kmemcpy(&VGA_BUF[(row - 1) * VGA_WIDTH],
                &VGA_BUF[row * VGA_WIDTH],
                VGA_WIDTH * sizeof(uint16_t));
    for (size_t col = 0; col < VGA_WIDTH; col++)
        VGA_BUF[(VGA_HEIGHT - 1) * VGA_WIDTH + col] = vga_entry(' ', tty_color);
}

void tty_init(void) {
    tty_row   = 0;
    tty_col   = 0;
    tty_color = vga_color(VGA_COLOR_LIGHT_GREY, VGA_COLOR_BLACK);
    for (size_t i = 0; i < VGA_WIDTH * VGA_HEIGHT; i++)
        VGA_BUF[i] = vga_entry(' ', tty_color);
    cursor_update();
}

void tty_setcolor(uint8_t color) {
    tty_color = color;
}

void tty_putchar(char c) {
    if (c == '\b') {
        if (tty_col > 0) tty_col--;
        VGA_BUF[tty_row * VGA_WIDTH + tty_col] = vga_entry(' ', tty_color);
        cursor_update();
        return;
    }
    if (c == '\n') {
        tty_col = 0;
        if (++tty_row == VGA_HEIGHT) {
            tty_scroll();
            tty_row = VGA_HEIGHT - 1;
        }
        cursor_update();
        return;
    }
    VGA_BUF[tty_row * VGA_WIDTH + tty_col] = vga_entry(c, tty_color);
    if (++tty_col == VGA_WIDTH) {
        tty_col = 0;
        if (++tty_row == VGA_HEIGHT) {
            tty_scroll();
            tty_row = VGA_HEIGHT - 1;
        }
    }
    cursor_update();
}

void tty_write(const char *str, size_t len) {
    for (size_t i = 0; i < len; i++)
        tty_putchar(str[i]);
}

void tty_print(const char *str) {
    tty_write(str, kstrlen(str));
}
