#include <stdint.h>
#include "include/shell.h"
#include "include/tty.h"
#include "include/vga.h"
#include "include/keyboard.h"
#include "include/pit.h"
#include "include/mem.h"

#define INPUT_MAX 256

static char input[INPUT_MAX];

static int kstrcmp(const char *a, const char *b) {
    while (*a && *a == *b) { a++; b++; }
    return *a - *b;
}

static void cmd_help(void) {
    tty_print("commands:\n");
    tty_print("  help    - show this message\n");
    tty_print("  clear   - clear the screen\n");
    tty_print("  uptime  - seconds since boot\n");
    tty_print("  echo    - echo text back\n");
}

static void cmd_clear(void) {
    tty_init();
}

static void cmd_uptime(void) {
    uint32_t secs = pit_ticks() / 1000;
    char buf[12];
    int i = 0;
    if (secs == 0) { buf[i++] = '0'; }
    else {
        uint32_t n = secs;
        int start = i;
        while (n) { buf[i++] = '0' + (n % 10); n /= 10; }
        for (int l = start, r = i - 1; l < r; l++, r--) {
            char tmp = buf[l]; buf[l] = buf[r]; buf[r] = tmp;
        }
    }
    buf[i] = '\0';
    tty_print(buf);
    tty_print("s\n");
}

static void cmd_echo(const char *args) {
    tty_print(args);
    tty_print("\n");
}

static void dispatch(void) {
    if (kstrcmp(input, "help") == 0)        cmd_help();
    else if (kstrcmp(input, "clear") == 0)  cmd_clear();
    else if (kstrcmp(input, "uptime") == 0) cmd_uptime();
    else if (input[0] == 'e' && input[1] == 'c' &&
             input[2] == 'h' && input[3] == 'o' && input[4] == ' ')
        cmd_echo(input + 5);
    else if (input[0] != '\0') {
        tty_setcolor(vga_color(VGA_COLOR_LIGHT_RED, VGA_COLOR_BLACK));
        tty_print("unknown command: ");
        tty_print(input);
        tty_print("\n");
        tty_setcolor(vga_color(VGA_COLOR_LIGHT_GREY, VGA_COLOR_BLACK));
    }
}

static void readline(void) {
    size_t i = 0;
    while (1) {
        char c = keyboard_getchar();
        if (c == '\n') {
            input[i] = '\0';
            tty_putchar('\n');
            return;
        }
        if (c == '\b' || c == 127) {
            if (i > 0) { i--; tty_putchar('\b'); }
            continue;
        }
        if (i < INPUT_MAX - 1) {
            input[i++] = c;
            tty_putchar(c);
        }
    }
}

void shell_run(void) {
    while (1) {
        tty_setcolor(vga_color(VGA_COLOR_LIGHT_GREEN, VGA_COLOR_BLACK));
        tty_print("$ ");
        tty_setcolor(vga_color(VGA_COLOR_WHITE, VGA_COLOR_BLACK));
        readline();
        dispatch();
    }
}
