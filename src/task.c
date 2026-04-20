#include <stdint.h>
#include <stddef.h>
#include "include/task.h"
#include "include/heap.h"
#include "include/mem.h"
#include "include/idt.h"
#include "include/pit.h"
#include "include/pic.h"
#include "include/kprintf.h"

static task_t   tasks[TASK_MAX];
static int      current = 0;
static int      ntasks  = 0;
static int      enabled = 0;

/*
 * context_switch(uint32_t *old_esp, uint32_t new_esp)
 * Saves callee-saved registers onto *old_esp, restores from new_esp.
 * Declared here; defined in task_switch.asm.
 */
extern void context_switch(uint32_t *old_esp, uint32_t new_esp);

static void task_exit(void) {
    tasks[current].state = TASK_DEAD;
    /* yield — the scheduler will skip dead tasks */
    for (;;) __asm__ volatile("hlt");
}

static void scheduler(registers_t *regs) {
    (void)regs;
    if (!enabled || ntasks < 2) { pic_eoi(0); return; }

    int prev = current;
    int next = current;
    for (int i = 1; i <= TASK_MAX; i++) {
        int candidate = (current + i) % TASK_MAX;
        if (tasks[candidate].state == TASK_READY ||
            tasks[candidate].state == TASK_RUNNING) {
            next = candidate;
            break;
        }
    }
    if (next == prev) { pic_eoi(0); return; }

    tasks[prev].state   = TASK_READY;
    tasks[next].state   = TASK_RUNNING;
    current             = next;

    pic_eoi(0);
    context_switch(&tasks[prev].esp, tasks[next].esp);
}

void task_init(void) {
    kmemset(tasks, 0, sizeof(tasks));
    /* Task 0 = current kernel execution context */
    tasks[0].state = TASK_RUNNING;
    kmemcpy(tasks[0].name, "kernel", 7);
    tasks[0].stack = NULL; /* uses the boot stack */
    ntasks  = 1;
    current = 0;
    idt_register_handler(32, scheduler);
    enabled = 1;
    kprintf("Tasks    ok\n");
}

task_t *task_create(const char *name, void (*entry)(void)) {
    for (int i = 0; i < TASK_MAX; i++) {
        if (tasks[i].state == TASK_DEAD || (i >= ntasks)) {
            uint32_t *stack = (uint32_t *)kmalloc(TASK_STACK_SIZE);
            if (!stack) return NULL;

            /* Build an initial stack frame for context_switch to restore.
               Stack grows down; push in reverse order.
               context_switch pops: edi, esi, ebx, ebp, then rets to eip. */
            uint32_t *sp = stack + TASK_STACK_SIZE / sizeof(uint32_t);
            *--sp = (uint32_t)task_exit;  /* return address if entry returns */
            *--sp = (uint32_t)entry;      /* eip (ret target of context_switch) */
            *--sp = 0;                    /* ebp */
            *--sp = 0;                    /* ebx */
            *--sp = 0;                    /* esi */
            *--sp = 0;                    /* edi */

            size_t nlen = kstrlen(name);
            if (nlen > 15) nlen = 15;
            kmemcpy(tasks[i].name, name, nlen);
            tasks[i].name[nlen] = '\0';
            tasks[i].stack = stack;
            tasks[i].esp   = (uint32_t)sp;
            tasks[i].state = TASK_READY;
            if (i >= ntasks) ntasks = i + 1;
            return &tasks[i];
        }
    }
    return NULL;
}

int task_count(void) {
    int n = 0;
    for (int i = 0; i < ntasks; i++)
        if (tasks[i].state != TASK_DEAD) n++;
    return n;
}

void task_foreach(void (*cb)(const task_t *)) {
    for (int i = 0; i < ntasks; i++)
        if (tasks[i].state != TASK_DEAD)
            cb(&tasks[i]);
}
