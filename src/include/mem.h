#pragma once
#include <stddef.h>

/**
 * @brief Fills @p n bytes of memory at @p dst with the byte value @p c.
 *
 * Freestanding replacement for the libc memset. Prefixed with @c k to avoid
 * conflicts with GCC compiler-generated intrinsic calls.
 *
 * @param dst Pointer to the memory region to fill.
 * @param c   Byte value to fill with.
 * @param n   Number of bytes to fill.
 * @return @p dst.
 */
void *kmemset(void *dst, int c, size_t n);

/**
 * @brief Copies @p n bytes from @p src to @p dst.
 *
 * Regions must not overlap. Freestanding replacement for the libc memcpy.
 *
 * @param dst Destination pointer.
 * @param src Source pointer.
 * @param n   Number of bytes to copy.
 * @return @p dst.
 */
void *kmemcpy(void *dst, const void *src, size_t n);

/**
 * @brief Returns the length of a null-terminated string, excluding the null.
 *
 * Freestanding replacement for the libc strlen.
 *
 * @param str Null-terminated string.
 * @return Number of characters before the null terminator.
 */
size_t kstrlen(const char *str);
