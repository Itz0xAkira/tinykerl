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
#include "include/serial.h"
#include "include/kprintf.h"
#include "include/shell.h"
#include "include/multiboot.h"

void kernel_main(uint32_t magic, multiboot_info_t *mb) {
    tty_init();
    serial_init();

    tty_setcolor(vga_color(VGA_COLOR_LIGHT_GREEN, VGA_COLOR_BLACK));
    kprintf("TinyKerl\n");
    tty_setcolor(vga_color(VGA_COLOR_LIGHT_GREY, VGA_COLOR_BLACK));

    gdt_init();     kprintf("GDT      ok\n");
    pic_init();
    idt_init();     kprintf("IDT      ok\n");
    pit_init(1000); kprintf("PIT      ok\n");
    keyboard_init();kprintf("Keyboard ok\n");
    serial_init();  kprintf("Serial   ok\n");

    if (magic == MULTIBOOT_MAGIC && (mb->flags & (1 << 6))) {
        pmm_init(mb->mmap_addr, mb->mmap_len);
        heap_init();
        kprintf("Memory   ok  (lower: %u KB  upper: %u KB)\n",
                mb->mem_lower, mb->mem_upper);
    } else {
        tty_setcolor(vga_color(VGA_COLOR_LIGHT_RED, VGA_COLOR_BLACK));
        kprintf("No memory map from bootloader\n");
        tty_setcolor(vga_color(VGA_COLOR_LIGHT_GREY, VGA_COLOR_BLACK));
    }

    __asm__ volatile("sti");
    kprintf("\n");
    shell_run();
}
