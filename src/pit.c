#include "include/pit.h"
#include "include/idt.h"
#include "include/pic.h"
#include "include/io.h"

#define PIT_HZ    1193182
#define PIT_CMD   0x43
#define PIT_DATA  0x40

static volatile uint32_t ticks = 0;

static void pit_irq(registers_t *regs) {
    (void)regs;
    ticks++;
    pic_eoi(0);
}

void pit_init(uint32_t hz) {
    uint32_t divisor = PIT_HZ / hz;
    outb(PIT_CMD, 0x36);
    outb(PIT_DATA, divisor & 0xFF);
    outb(PIT_DATA, (divisor >> 8) & 0xFF);
    idt_register_handler(32, pit_irq);
    pic_unmask(0);
}

uint32_t pit_ticks(void) {
    return ticks;
}

void pit_sleep(uint32_t ms) {
    uint32_t end = ticks + ms;
    while (ticks < end) __asm__ volatile("hlt");
}
