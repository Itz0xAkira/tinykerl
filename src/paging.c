#include <stdint.h>
#include "include/paging.h"
#include "include/pmm.h"
#include "include/mem.h"
#include "include/kprintf.h"

#define PDE_PRESENT  (1 << 0)
#define PDE_WRITABLE (1 << 1)
#define PTE_PRESENT  (1 << 0)
#define PTE_WRITABLE (1 << 1)

extern uint32_t _kernel_end;

void paging_init(void) {
    uint32_t *page_dir = (uint32_t *)pmm_alloc();
    kmemset(page_dir, 0, PAGE_SIZE);

    uint32_t map_end = ((uint32_t)&_kernel_end + PAGE_SIZE - 1) & ~(PAGE_SIZE - 1);
    /* Always map at least the first 4 MB so VGA (0xB8000) is reachable */
    if (map_end < 4 * 1024 * 1024)
        map_end = 4 * 1024 * 1024;

    for (uint32_t addr = 0; addr < map_end; addr += PAGE_SIZE) {
        uint32_t pdi = addr >> 22;
        uint32_t pti = (addr >> 12) & 0x3FF;

        if (!(page_dir[pdi] & PDE_PRESENT)) {
            uint32_t *pt = (uint32_t *)pmm_alloc();
            kmemset(pt, 0, PAGE_SIZE);
            page_dir[pdi] = (uint32_t)pt | PDE_PRESENT | PDE_WRITABLE;
        }

        uint32_t *pt = (uint32_t *)(page_dir[pdi] & ~0xFFF);
        pt[pti] = addr | PTE_PRESENT | PTE_WRITABLE;
    }

    __asm__ volatile(
        "mov %0, %%cr3\n"
        "mov %%cr0, %%eax\n"
        "or  $0x80000000, %%eax\n"
        "mov %%eax, %%cr0\n"
        :: "r"(page_dir) : "eax"
    );

    kprintf("Paging   ok  (mapped %u KB)\n", map_end / 1024);
}
