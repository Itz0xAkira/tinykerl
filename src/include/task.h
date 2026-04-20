#pragma once
#include <stdint.h>

#define TASK_STACK_SIZE 4096
#define TASK_MAX        8

typedef enum {
    TASK_READY,
    TASK_RUNNING,
    TASK_DEAD,
} task_state_t;

typedef struct {
    uint32_t     esp;        /* saved stack pointer */
    uint32_t    *stack;      /* base of allocated stack */
    task_state_t state;
    char         name[16];
} task_t;

/**
 * @brief Initialises the task subsystem and creates the idle (kernel) task.
 *
 * Must be called after paging_init() and pit_init().
 */
void task_init(void);

/**
 * @brief Creates a new kernel thread that will run @p entry.
 *
 * @param name  Short display name (truncated to 15 chars).
 * @param entry Function to run as the thread body.
 * @return Pointer to the new task_t, or NULL if the table is full.
 */
task_t *task_create(const char *name, void (*entry)(void));

/**
 * @brief Returns the current number of live tasks.
 */
int task_count(void);

/**
 * @brief Iterates live tasks, calling @p cb for each.
 */
void task_foreach(void (*cb)(const task_t *));
