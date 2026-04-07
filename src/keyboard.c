#include <stdbool.h>
#include "include/keyboard.h"
#include "include/idt.h"
#include "include/pic.h"
#include "include/io.h"

#define KB_DATA 0x60
#define BUF_SZ  256

static const char sc_ascii[] = {
    0,   0,  '1','2','3','4','5','6','7','8','9','0','-','=', 0,
   '\t','q','w','e','r','t','y','u','i','o','p','[',']','\n',
    0,  'a','s','d','f','g','h','j','k','l',';','\'','`',
    0, '\\','z','x','c','v','b','n','m',',','.','/', 0,
   '*', 0,  ' '
};

static const char sc_ascii_shift[] = {
    0,   0,  '!','@','#','$','%','^','&','*','(',')','_','+', 0,
   '\t','Q','W','E','R','T','Y','U','I','O','P','{','}','\n',
    0,  'A','S','D','F','G','H','J','K','L',':','"','~',
    0,  '|','Z','X','C','V','B','N','M','<','>','?', 0,
   '*', 0,  ' '
};

static char    buf[BUF_SZ];
static uint8_t buf_head = 0;
static uint8_t buf_tail = 0;
static bool    shift    = false;

static void kb_irq(registers_t *regs) {
    (void)regs;
    uint8_t sc = inb(KB_DATA);

    if (sc == 0x2A || sc == 0x36) { shift = true;  pic_eoi(1); return; }
    if (sc == 0xAA || sc == 0xB6) { shift = false; pic_eoi(1); return; }
    if (sc & 0x80) { pic_eoi(1); return; }

    if (sc < sizeof(sc_ascii)) {
        char c = shift ? sc_ascii_shift[sc] : sc_ascii[sc];
        if (c) {
            buf[buf_head] = c;
            buf_head = (buf_head + 1) % BUF_SZ;
        }
    }
    pic_eoi(1);
}

void keyboard_init(void) {
    idt_register_handler(33, kb_irq);
    pic_unmask(1);
}

bool keyboard_haschar(void) {
    return buf_head != buf_tail;
}

char keyboard_getchar(void) {
    while (!keyboard_haschar()) __asm__ volatile("hlt");
    char c = buf[buf_tail];
    buf_tail = (buf_tail + 1) % BUF_SZ;
    return c;
}
