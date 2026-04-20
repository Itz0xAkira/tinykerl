#pragma once
#include <stdint.h>
#include "idt.h"

/* Syscall numbers (eax) */
#define SYS_EXIT  60
#define SYS_WRITE  1

/**
 * @brief Registers ISR 128 (int 0x80) and sets up the syscall dispatch table.
 */
void syscall_init(void);
