section .asm


extern int21_handler
extern no_int_handler

global idt_load
global int21h
global no_inth

idt_load:
    push ebp

    mov ebp, esp
    mov ebx, [ebp + 8]
    lidt[ebx]

    pop  ebp
    ret

int21h:

    cli

    pushad
    call int21_handler
    popad

    sti
    iret

no_inth:

    cli

    pushad
    call no_int_handler
    popad

    sti
    iret
