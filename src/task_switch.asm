global context_switch

; void context_switch(uint32_t *old_esp, uint32_t new_esp)
; [esp+4] = old_esp (pointer to where we save the old stack pointer)
; [esp+8] = new_esp (stack pointer to restore)
context_switch:
    ; save callee-saved registers onto the current stack
    push ebp
    push ebx
    push esi
    push edi

    ; save current esp into *old_esp
    mov eax, [esp+20]       ; old_esp argument (4 regs pushed = 16 bytes + ret = 20)
    mov [eax], esp

    ; switch to new stack
    mov esp, [esp+24]       ; new_esp argument

    ; restore callee-saved registers from new stack
    pop edi
    pop esi
    pop ebx
    pop ebp

    ret                     ; jumps to the saved eip of the new task
