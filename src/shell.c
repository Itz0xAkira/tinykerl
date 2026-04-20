#include <stdint.h>
#include <stddef.h>
#include "include/shell.h"
#include "include/tty.h"
#include "include/vga.h"
#include "include/keyboard.h"
#include "include/pit.h"
#include "include/pmm.h"
#include "include/kprintf.h"
#include "include/io.h"
#include "include/mem.h"

#define INPUT_MAX 256

static char input[INPUT_MAX];

static int kstrcmp(const char *a, const char *b) {
    while (*a && *a == *b) { a++; b++; }
    return *a - *b;
}

static int kstrncmp(const char *a, const char *b, size_t n) {
    while (n-- && *a && *a == *b) { a++; b++; }
    return n == (size_t)-1 ? 0 : *a - *b;
}

static void cmd_help(void) {
    kprintf("commands:\n");
    kprintf("  help    - show this message\n");
    kprintf("  clear   - clear the screen\n");
    kprintf("  uptime  - time since boot\n");
    kprintf("  echo    - echo text back\n");
    kprintf("  mem     - show memory usage\n");
    kprintf("  reboot  - reboot the system\n");
    kprintf("  halt    - halt the system\n");
}

static void cmd_clear(void) {
    tty_init();
}

static void cmd_uptime(void) {
    uint32_t ms   = pit_ticks();
    uint32_t secs = ms / 1000;
    uint32_t mins = secs / 60;
    uint32_t hrs  = mins / 60;
    kprintf("%uh %um %us\n", hrs, mins % 60, secs % 60);
}

static void cmd_mem(void) {
    uint32_t free_pages = pmm_free_count();
    kprintf("free:  %u KB\n", free_pages * 4);
}

static void cmd_reboot(void) {
    uint8_t good = 0x02;
    while (good & 0x02) good = inb(0x64);
    outb(0x64, 0xFE);
    for (;;) __asm__ volatile("hlt");
}

static void cmd_halt(void) {
    kprintf("System halted.\n");
    __asm__ volatile("cli");
    for (;;) __asm__ volatile("hlt");
}

static void cmd_echo(const char *args) {
    kprintf("%s\n", args);
}

typedef struct {
    const char *name;
    void (*fn)(const char *args);
    int takes_args;
} cmd_t;

static void cmd_help_wrap(const char *a)    { (void)a; cmd_help(); }
static void cmd_clear_wrap(const char *a)   { (void)a; cmd_clear(); }
static void cmd_uptime_wrap(const char *a)  { (void)a; cmd_uptime(); }
static void cmd_mem_wrap(const char *a)     { (void)a; cmd_mem(); }
static void cmd_reboot_wrap(const char *a)  { (void)a; cmd_reboot(); }
static void cmd_halt_wrap(const char *a)    { (void)a; cmd_halt(); }

static const cmd_t cmds[] = {
    {"help",   cmd_help_wrap,   0},
    {"clear",  cmd_clear_wrap,  0},
    {"uptime", cmd_uptime_wrap, 0},
    {"mem",    cmd_mem_wrap,    0},
    {"reboot", cmd_reboot_wrap, 0},
    {"halt",   cmd_halt_wrap,   0},
    {"echo",   cmd_echo,        1},
};
#define NCMDS (sizeof(cmds) / sizeof(cmds[0]))

static void dispatch(void) {
    for (size_t i = 0; i < NCMDS; i++) {
        size_t nlen = kstrlen(cmds[i].name);
        if (cmds[i].takes_args) {
            if (kstrncmp(input, cmds[i].name, nlen) == 0 && input[nlen] == ' ') {
                cmds[i].fn(input + nlen + 1);
                return;
            }
        } else {
            if (kstrcmp(input, cmds[i].name) == 0) {
                cmds[i].fn(NULL);
                return;
            }
        }
    }
    if (input[0] != '\0') {
        tty_setcolor(vga_color(VGA_COLOR_LIGHT_RED, VGA_COLOR_BLACK));
        kprintf("unknown command: %s\n", input);
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
        if (c == '\b') {
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
