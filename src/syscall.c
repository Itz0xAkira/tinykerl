#include <stdint.h>
#include "include/syscall.h"
#include "include/idt.h"
#include "include/vfs.h"
#include "include/kprintf.h"
#include "include/tty.h"
#include "include/serial.h"

/*
 * Calling convention (Linux-compatible subset):
 *   eax = syscall number
 *   ebx = arg1, ecx = arg2, edx = arg3
 *   return value written back to eax in the saved register frame
 */

static void syscall_handler(registers_t *regs) {
    int ret = -1;
    switch (regs->eax) {
        case SYS_WRITE: {
            /* ebx = fd (1=stdout), ecx = buf ptr, edx = len */
            const char *buf = (const char *)regs->ecx;
            uint32_t    len = regs->edx;
            if (regs->ebx == 1) {
                for (uint32_t i = 0; i < len; i++) {
                    tty_putchar(buf[i]);
                    serial_putchar(buf[i]);
                }
                ret = (int)len;
            } else {
                ret = vfs_write((int)regs->ebx, buf, len);
            }
            break;
        }
        case SYS_EXIT:
            kprintf("[task exited with code %d]\n", (int)regs->ebx);
            /* In a real kernel we'd kill the current task; for now just hang. */
            for (;;) __asm__ volatile("hlt");
            break;
        default:
            kprintf("unknown syscall %u\n", regs->eax);
            break;
    }
    regs->eax = (uint32_t)ret;
}

void syscall_init(void) {
    idt_register_handler(128, syscall_handler);
    kprintf("Syscall  ok  (int 0x80)\n");
}
