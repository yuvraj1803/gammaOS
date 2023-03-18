section .asm


extern no_int_handler

global idt_load
global no_inth

; load idtr into processor
idt_load:
    push ebp

    mov ebp, esp
    mov ebx, [ebp + 8]
    lidt[ebx]

    pop  ebp
    ret

no_inth:

    cli

    pushad
    call no_int_handler
    popad

    sti
    iret
