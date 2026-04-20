#pragma once
#include <stdint.h>

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
