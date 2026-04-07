#pragma once

/**
 * @brief Starts the interactive kernel shell.
 *
 * Enters an infinite loop that prints a prompt, reads a line from the
 * keyboard, and dispatches to the matching command handler.
 *
 * Built-in commands: help, clear, uptime, echo.
 *
 * @note This function never returns. Call it last in kernel_main after all
 *       subsystems are initialized and interrupts are enabled dont be an IDIOT.
 */
void shell_run(void);
