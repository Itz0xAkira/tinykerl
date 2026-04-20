#include <stdint.h>
#include <stddef.h>
#include "include/pmm.h"
#include "include/mem.h"

#define MAX_PAGES 32768

static uint32_t bitmap[MAX_PAGES / 32];
static uint32_t total_pages = 0;

extern uint32_t _kernel_end;

static void bitmap_set(uint32_t page)   { bitmap[page / 32] |=  (1 << (page % 32)); }
static void bitmap_clear(uint32_t page) { bitmap[page / 32] &= ~(1 << (page % 32)); }
static int  bitmap_test(uint32_t page)  { return bitmap[page / 32] &  (1 << (page % 32)); }

void pmm_init(uint32_t mmap_addr, uint32_t mmap_len) {
    kmemset(bitmap, 0xFF, sizeof(bitmap));

    uint32_t kernel_end = (uint32_t)&_kernel_end;
    mmap_entry_t *entry = (mmap_entry_t *)mmap_addr;

    while ((uint32_t)entry < mmap_addr + mmap_len) {
        if (entry->type == 1 && entry->base_high == 0) {
            uint32_t base  = entry->base_low;
            uint32_t limit = entry->len_low;
            for (uint32_t addr = base; addr < base + limit; addr += PAGE_SIZE) {
                uint32_t page = addr / PAGE_SIZE;
                if (page < MAX_PAGES && addr >= kernel_end)
                    bitmap_clear(page);
            }
            total_pages += limit / PAGE_SIZE;
        }
        entry = (mmap_entry_t *)((uint32_t)entry + entry->size + 4);
    }
}

void *pmm_alloc(void) {
    for (uint32_t i = 0; i < MAX_PAGES; i++) {
        if (!bitmap_test(i)) {
            bitmap_set(i);
            return (void *)(i * PAGE_SIZE);
        }
    }
    return NULL;
}

void pmm_free(void *addr) {
    bitmap_clear((uint32_t)addr / PAGE_SIZE);
}

uint32_t pmm_free_count(void) {
    uint32_t count = 0;
    for (uint32_t i = 0; i < MAX_PAGES; i++)
        if (!bitmap_test(i)) count++;
    return count;
}
