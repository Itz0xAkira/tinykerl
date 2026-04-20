#pragma once
#include <stdint.h>

/**
 * @brief Identity-maps the kernel (0 → _kernel_end) and enables CR0.PG.
 *
 * Allocates a page directory and enough page tables from the PMM to cover
 * all physical pages from 0 up to and including the page containing
 * _kernel_end.  Every mapped virtual address equals its physical address.
 * After this call paging is active.
 */
void paging_init(void);
