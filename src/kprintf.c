#include <stdarg.h>
#include <stdint.h>
#include "include/kprintf.h"
#include "include/tty.h"
#include "include/serial.h"

static void print_str(const char *s) {
    tty_print(s);
    serial_print(s);
}

static void print_char(char c) {
    tty_putchar(c);
    serial_putchar(c);
}

static void print_uint(uint32_t n, uint32_t base) {
    static const char digits[] = "0123456789abcdef";
    char buf[32];
    int i = 0;
    if (n == 0) { print_char('0'); return; }
    while (n) { buf[i++] = digits[n % base]; n /= base; }
    while (i--) print_char(buf[i]);
}

static void print_int(int32_t n) {
    if (n < 0) { print_char('-'); print_uint((uint32_t)-n, 10); }
    else print_uint((uint32_t)n, 10);
}

void kvprintf(const char *fmt, va_list args) {
    for (; *fmt; fmt++) {
        if (*fmt != '%') { print_char(*fmt); continue; }
        switch (*++fmt) {
            case 'c': print_char((char)va_arg(args, int));        break;
            case 's': print_str(va_arg(args, const char *));      break;
            case 'd': print_int(va_arg(args, int32_t));           break;
            case 'u': print_uint(va_arg(args, uint32_t), 10);     break;
            case 'x': print_uint(va_arg(args, uint32_t), 16);     break;
            case '%': print_char('%');                             break;
            default:  print_char('%'); print_char(*fmt);          break;
        }
    }
}

void kprintf(const char *fmt, ...) {
    va_list args;
    va_start(args, fmt);
    kvprintf(fmt, args);
    va_end(args);
}
