#pragma once
#include <stdint.h>
#include <stddef.h>

/**
 * @brief Returns the number of free physical pages.
 *
 * @return Count of free 4KB pages currently in the PMM bitmap.
 */
uint32_t pmm_free_count(void);

/**
 * @brief A single entry from the GRUB-provided memory map.
 *
 * The @p type field indicates the region's usability:
 * - 1 = available RAM
 * - 2 = reserved
 * - 3 = ACPI reclaimable
 * - 4 = ACPI NVS
 * - 5 = bad memory
 */
typedef struct {
    uint32_t size;
    uint32_t base_low;  /**< Lower 32 bits of the region's base address. */
    uint32_t base_high; /**< Upper 32 bits of the region's base address. */
    uint32_t len_low;   /**< Lower 32 bits of the region's length in bytes. */
    uint32_t len_high;  /**< Upper 32 bits of the region's length in bytes. */
    uint32_t type;      /**< Region type (1 = usable). */
} __attribute__((packed)) mmap_entry_t;

/**
 * @brief Initializes the physical memory manager from the multiboot memory map.
 *
 * Marks all pages as used, then walks the memory map and clears bits for
 * pages that are both available and above the end of the kernel image.
 *
 * @param mmap_addr Physical address of the multiboot mmap buffer.
 * @param mmap_len  Length in bytes of the multiboot mmap buffer.
 */
void pmm_init(uint32_t mmap_addr, uint32_t mmap_len);

/**
 * @brief Allocates one 4KB physical page frame.
 *
 * Scans the bitmap for the first free page, marks it used, and returns its
 * physical address.
 *
 * @return Physical address of the allocated page, or NULL if out of memory.
 */
void *pmm_alloc(void);

/**
 * @brief Frees a previously allocated 4KB page frame.
 *
 * @param addr Physical address returned by pmm_alloc().
 */
void pmm_free(void *addr);
