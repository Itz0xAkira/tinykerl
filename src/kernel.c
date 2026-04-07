#include <stdint.h>
#include "include/tty.h"
#include "include/vga.h"
#include "include/gdt.h"
#include "include/idt.h"
#include "include/pic.h"
#include "include/pit.h"
#include "include/keyboard.h"
#include "include/pmm.h"
#include "include/heap.h"
#include "include/shell.h"

#define MULTIBOOT_MAGIC 0x2BADB002

typedef struct __attribute__((packed)) {
    uint32_t flags;
    uint32_t mem_lower, mem_upper;
    uint32_t boot_device;
    uint32_t cmdline;
    uint32_t mods_count, mods_addr;
    uint32_t syms[4];
    uint32_t mmap_len, mmap_addr;
} multiboot_info_t;

void kernel_main(uint32_t magic, multiboot_info_t *mb) {
    tty_init();

    tty_setcolor(vga_color(VGA_COLOR_LIGHT_GREEN, VGA_COLOR_BLACK));
    tty_print("TinyKerl\n");
    tty_setcolor(vga_color(VGA_COLOR_LIGHT_GREY, VGA_COLOR_BLACK));

    gdt_init();
    tty_print("GDT ok\n");

    pic_init();
    idt_init();
    tty_print("IDT ok\n");

    pit_init(1000);
    tty_print("PIT ok\n");

    keyboard_init();
    tty_print("Keyboard ok\n");

    if (magic == MULTIBOOT_MAGIC && (mb->flags & (1 << 6))) {
        pmm_init(mb->mmap_addr, mb->mmap_len);
        heap_init();
        tty_print("Memory ok\n");
    } else {
        tty_setcolor(vga_color(VGA_COLOR_LIGHT_RED, VGA_COLOR_BLACK));
        tty_print("No memory map from bootloader\n");
        tty_setcolor(vga_color(VGA_COLOR_LIGHT_GREY, VGA_COLOR_BLACK));
    }

    __asm__ volatile("sti");

    tty_print("\n");
    shell_run();
}
