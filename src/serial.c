#include "include/serial.h"
#include "include/io.h"
#include "include/mem.h"

#define COM1 0x3F8

int serial_init(void) {
    outb(COM1 + 1, 0x00);
    outb(COM1 + 3, 0x80);
    outb(COM1 + 0, 0x03);
    outb(COM1 + 1, 0x00);
    outb(COM1 + 3, 0x03);
    outb(COM1 + 2, 0xC7);
    outb(COM1 + 4, 0x0B);
    outb(COM1 + 4, 0x1E);
    outb(COM1 + 0, 0xAE);

    if (inb(COM1 + 0) != 0xAE) return 0;

    outb(COM1 + 4, 0x0F);
    return 1;
}

void serial_putchar(char c) {
    while (!(inb(COM1 + 5) & 0x20));
    outb(COM1, c);
}

void serial_print(const char *str) {
    for (size_t i = 0; str[i]; i++)
        serial_putchar(str[i]);
}
