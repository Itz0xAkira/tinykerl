#pragma once
#include <stdint.h>

/**
 * @brief Writes a byte to an x86 I/O port.
 *
 * @param port I/O port address.
 * @param val  Byte value to write.
 */
static inline void outb(uint16_t port, uint8_t val) {
    __asm__ volatile("outb %0, %1" : : "a"(val), "Nd"(port));
}

/**
 * @brief Reads a byte from an x86 I/O port.
 *
 * @param port I/O port address.
 * @return Byte read from the port.
 */
static inline uint8_t inb(uint16_t port) {
    uint8_t ret;
    __asm__ volatile("inb %1, %0" : "=a"(ret) : "Nd"(port));
    return ret;
}
