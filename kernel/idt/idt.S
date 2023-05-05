section .asm


extern general_execption_handler
extern isr_0x80_handler
extern ps2_interrupt_handler    

global idt_load
global general_execption
global isr_0x80
global isr_0x21

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

; no_inth is called when an interrupt whose handler has not be defined yet.
; so basically, its a default interrupt handler
general_execption:

    cli

    pushad
    call general_execption_handler
    popad

    sti
    iret


; isr_0x80 is pretty self-explanatory, its for the system call interrupts
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


isr_0x21:
    pushad
    call ps2_interrupt_handler
    popad
    
    iret

section .data

temp_ret_val: dd 0 ; temporary variable used to store return value of isr_0x80_handler