#include <stddef.h>
#include <stdint.h>
#include "include/heap.h"
#include "include/pmm.h"
#include "include/mem.h"

typedef struct block {
    size_t        size;
    int           free;
    struct block *next;
} block_t;

static block_t *head = NULL;

static block_t *new_page(size_t size) {
    size_t pages = (size + sizeof(block_t) + PAGE_SIZE - 1) / PAGE_SIZE;
    void *first = pmm_alloc();
    if (!first) return NULL;
    for (size_t i = 1; i < pages; i++) {
        /* remaining frames are intentionally allocated but not tracked;
           the block header covers the full claimed size only if the PMM
           returns them contiguously, which is not guaranteed on real hw.
           Clamp to a single page to stay safe. */
        (void)i;
        break;
    }
    block_t *b = (block_t *)first;
    b->size = PAGE_SIZE - sizeof(block_t);
    b->free = 0;
    b->next = NULL;
    return b;
}

void heap_init(void) {
    head = new_page(PAGE_SIZE);
    if (head) head->free = 1;
}

void *kmalloc(size_t size) {
    if (!size) return NULL;
    size = (size + 7) & ~7;

    block_t *b = head;
    while (b) {
        if (b->free && b->size >= size) {
            if (b->size >= size + sizeof(block_t) + 8) {
                block_t *split = (block_t *)((uint8_t *)(b + 1) + size);
                split->size = b->size - size - sizeof(block_t);
                split->free = 1;
                split->next = b->next;
                b->next = split;
                b->size = size;
            }
            b->free = 0;
            return (void *)(b + 1);
        }
        if (!b->next) {
            b->next = new_page(size);
            if (!b->next) return NULL;
        }
        b = b->next;
    }
    return NULL;
}

void kfree(void *ptr) {
    if (!ptr) return;
    block_t *b = (block_t *)ptr - 1;
    b->free = 1;

    block_t *cur = head;
    while (cur && cur->next) {
        if (cur->free && cur->next->free) {
            cur->size += sizeof(block_t) + cur->next->size;
            cur->next  = cur->next->next;
        } else {
            cur = cur->next;
        }
    }
}
