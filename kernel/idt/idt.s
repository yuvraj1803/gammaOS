section .asm


extern no_int_handler
extern isr_0x80_handler

global idt_load
global no_inth
global isr_0x80

global __enable_irq
global __disable_irq

; enable interrupts
__enable_irq:
    sti
    ret

; disable interrupts
__disable_irq:
    cli
    ret

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

isr_0x80:

    pushad
    push esp
    push eax

    call isr_0x80_handler

    mov dword[temp_ret_val], eax
    add esp, 8
    popad

    mov eax, dword[temp_ret_val]

    iret


section .data

temp_ret_val: dd 0 ; temporary variable used to store return value of isr_0x80_handler