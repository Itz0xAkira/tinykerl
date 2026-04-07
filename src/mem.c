#include <stddef.h>

void *kmemset(void *dst, int c, size_t n) {
    unsigned char *p = dst;
    while (n--) *p++ = (unsigned char)c;
    return dst;
}

void *kmemcpy(void *dst, const void *src, size_t n) {
    unsigned char *d = dst;
    const unsigned char *s = src;
    while (n--) *d++ = *s++;
    return dst;
}

size_t kstrlen(const char *str) {
    size_t len = 0;
    while (str[len]) len++;
    return len;
}
