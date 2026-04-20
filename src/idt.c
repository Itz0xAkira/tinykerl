#include <stdint.h>
#include "include/idt.h"
#include "include/tty.h"
#include "include/kprintf.h"

typedef struct __attribute__((packed)) {
    uint16_t base_low;
    uint16_t sel;
    uint8_t  zero;
    uint8_t  flags;
    uint16_t base_high;
} idt_entry_t;

typedef struct __attribute__((packed)) {
    uint16_t limit;
    uint32_t base;
} idt_ptr_t;

static idt_entry_t  idt[256];
static idt_ptr_t    idt_ptr;
static isr_handler_t handlers[256];

extern void idt_flush(uint32_t);

extern void isr0(void);  extern void isr1(void);  extern void isr2(void);
extern void isr3(void);  extern void isr4(void);  extern void isr5(void);
extern void isr6(void);  extern void isr7(void);  extern void isr8(void);
extern void isr9(void);  extern void isr10(void); extern void isr11(void);
extern void isr12(void); extern void isr13(void); extern void isr14(void);
extern void isr15(void); extern void isr16(void); extern void isr17(void);
extern void isr18(void); extern void isr19(void); extern void isr20(void);
extern void isr21(void); extern void isr22(void); extern void isr23(void);
extern void isr24(void); extern void isr25(void); extern void isr26(void);
extern void isr27(void); extern void isr28(void); extern void isr29(void);
extern void isr30(void); extern void isr31(void);

extern void irq0(void);  extern void irq1(void);  extern void irq2(void);
extern void irq3(void);  extern void irq4(void);  extern void irq5(void);
extern void irq6(void);  extern void irq7(void);  extern void irq8(void);
extern void irq9(void);  extern void irq10(void); extern void irq11(void);
extern void irq12(void); extern void irq13(void); extern void irq14(void);
extern void irq15(void);

static void idt_set(uint8_t n, uint32_t base, uint16_t sel, uint8_t flags) {
    idt[n].base_low  = base & 0xFFFF;
    idt[n].base_high = (base >> 16) & 0xFFFF;
    idt[n].sel       = sel;
    idt[n].zero      = 0;
    idt[n].flags     = flags;
}

static const char *exception_names[] = {
    "Division By Zero", "Debug", "NMI", "Breakpoint",
    "Overflow", "Bound Range Exceeded", "Invalid Opcode", "Device Not Available",
    "Double Fault", "Coprocessor Segment Overrun", "Invalid TSS", "Segment Not Present",
    "Stack Fault", "General Protection Fault", "Page Fault", "Reserved",
    "x87 FPU Error", "Alignment Check", "Machine Check", "SIMD FP Exception",
    "Virtualization Exception", "Reserved", "Reserved", "Reserved",
    "Reserved", "Reserved", "Reserved", "Reserved",
    "Reserved", "Reserved", "Security Exception", "Reserved"
};

void isr_handler(registers_t *regs) {
    if (handlers[regs->int_no]) {
        handlers[regs->int_no](regs);
        return;
    }
    if (regs->int_no == 14) {
        uint32_t cr2;
        __asm__ volatile("mov %%cr2, %0" : "=r"(cr2));
        kpanic("Page Fault at 0x%x (err=0x%x) eip=0x%x\n",
               cr2, regs->err_code, regs->eip);
    }
    kpanic("Exception %u: %s (err=0x%x) eip=0x%x\n",
           regs->int_no,
           regs->int_no < 32 ? exception_names[regs->int_no] : "Unknown",
           regs->err_code, regs->eip);
}

void irq_handler(registers_t *regs) {
    if (handlers[regs->int_no])
        handlers[regs->int_no](regs);
}

void idt_register_handler(uint8_t n, isr_handler_t handler) {
    handlers[n] = handler;
}

void idt_init(void) {
    idt_ptr.limit = sizeof(idt) - 1;
    idt_ptr.base  = (uint32_t)&idt;

    idt_set(0,  (uint32_t)isr0,  0x08, 0x8E);
    idt_set(1,  (uint32_t)isr1,  0x08, 0x8E);
    idt_set(2,  (uint32_t)isr2,  0x08, 0x8E);
    idt_set(3,  (uint32_t)isr3,  0x08, 0x8E);
    idt_set(4,  (uint32_t)isr4,  0x08, 0x8E);
    idt_set(5,  (uint32_t)isr5,  0x08, 0x8E);
    idt_set(6,  (uint32_t)isr6,  0x08, 0x8E);
    idt_set(7,  (uint32_t)isr7,  0x08, 0x8E);
    idt_set(8,  (uint32_t)isr8,  0x08, 0x8E);
    idt_set(9,  (uint32_t)isr9,  0x08, 0x8E);
    idt_set(10, (uint32_t)isr10, 0x08, 0x8E);
    idt_set(11, (uint32_t)isr11, 0x08, 0x8E);
    idt_set(12, (uint32_t)isr12, 0x08, 0x8E);
    idt_set(13, (uint32_t)isr13, 0x08, 0x8E);
    idt_set(14, (uint32_t)isr14, 0x08, 0x8E);
    idt_set(15, (uint32_t)isr15, 0x08, 0x8E);
    idt_set(16, (uint32_t)isr16, 0x08, 0x8E);
    idt_set(17, (uint32_t)isr17, 0x08, 0x8E);
    idt_set(18, (uint32_t)isr18, 0x08, 0x8E);
    idt_set(19, (uint32_t)isr19, 0x08, 0x8E);
    idt_set(20, (uint32_t)isr20, 0x08, 0x8E);
    idt_set(21, (uint32_t)isr21, 0x08, 0x8E);
    idt_set(22, (uint32_t)isr22, 0x08, 0x8E);
    idt_set(23, (uint32_t)isr23, 0x08, 0x8E);
    idt_set(24, (uint32_t)isr24, 0x08, 0x8E);
    idt_set(25, (uint32_t)isr25, 0x08, 0x8E);
    idt_set(26, (uint32_t)isr26, 0x08, 0x8E);
    idt_set(27, (uint32_t)isr27, 0x08, 0x8E);
    idt_set(28, (uint32_t)isr28, 0x08, 0x8E);
    idt_set(29, (uint32_t)isr29, 0x08, 0x8E);
    idt_set(30, (uint32_t)isr30, 0x08, 0x8E);
    idt_set(31, (uint32_t)isr31, 0x08, 0x8E);

    idt_set(32, (uint32_t)irq0,  0x08, 0x8E);
    idt_set(33, (uint32_t)irq1,  0x08, 0x8E);
    idt_set(34, (uint32_t)irq2,  0x08, 0x8E);
    idt_set(35, (uint32_t)irq3,  0x08, 0x8E);
    idt_set(36, (uint32_t)irq4,  0x08, 0x8E);
    idt_set(37, (uint32_t)irq5,  0x08, 0x8E);
    idt_set(38, (uint32_t)irq6,  0x08, 0x8E);
    idt_set(39, (uint32_t)irq7,  0x08, 0x8E);
    idt_set(40, (uint32_t)irq8,  0x08, 0x8E);
    idt_set(41, (uint32_t)irq9,  0x08, 0x8E);
    idt_set(42, (uint32_t)irq10, 0x08, 0x8E);
    idt_set(43, (uint32_t)irq11, 0x08, 0x8E);
    idt_set(44, (uint32_t)irq12, 0x08, 0x8E);
    idt_set(45, (uint32_t)irq13, 0x08, 0x8E);
    idt_set(46, (uint32_t)irq14, 0x08, 0x8E);
    idt_set(47, (uint32_t)irq15, 0x08, 0x8E);

    idt_flush((uint32_t)&idt_ptr);
}
