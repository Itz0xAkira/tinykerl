#pragma once
#include <stdint.h>

/**
 * @brief Initializes the Global Descriptor Table.
 *
 * Sets up a flat 32-bit memory model with three segments: null, kernel code
 * (0x08), and kernel data (0x10). Loads the GDT and reloads all segment
 * registers via gdt_flush.
 */
void gdt_init(void);
