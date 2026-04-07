#pragma once
#include <stddef.h>

/**
 * @brief Initializes the kernel heap.
 *
 * Allocates an initial page from the PMM and sets up the free-list head.
 * Must be called after pmm_init().
 */
void heap_init(void);

/**
 * @brief Allocates at least @p size bytes from the kernel heap.
 *
 * Walks the free list for a suitable block, splitting it if large enough.
 * Requests a new page from the PMM if no free block fits.
 *
 * @param size Number of bytes to allocate.
 * @return Pointer to allocated memory, or NULL if out of memory.
 */
void *kmalloc(size_t size);

/**
 * @brief Frees a block previously returned by kmalloc().
 *
 * Marks the block as free and coalesces adjacent free blocks to reduce
 * fragmentation.
 *
 * @param ptr Pointer returned by kmalloc(). Passing NULL is a no-op.
 */
void kfree(void *ptr);
