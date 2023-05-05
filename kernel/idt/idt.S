
section .asm


extern no_int_handler
extern isr_0x80_handler
extern ps2_interrupt_handler    
extern general_interrupt_handler

global idt_load
global no_int
global isr_0x80
global interrupt_pointer_table_entries

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

; no_int is called when an interrupt whose handler has not be defined yet.
; so basically, its a default interrupt handler
no_int:

    cli

    pushad
    call no_int_handler
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



; Generalising all 512 interrupt service routines
%macro interrupt 1
    global isr%1
    isr%1:
        pushad
        push esp ; pushing the interrupt frame
        push dword %1 ; pushing the interrupt number
        call general_interrupt_handler ; this function defined in idt/idt.c acknowledges the pushed values and calls their respective handlers.
        add esp, 8
        popad
        iret
%endmacro

%assign i 0
%rep 512
    interrupt i
%assign i i+1
%endrep



section .data

temp_ret_val: dd 0 ; temporary variable used to store return value of isr_0x80_handler


;; This is an array of addresses of the interrupt service routines with the label : interrupt_pointer_table_entry 'i'
; which has the address of the isr of interrupt 'i'
%macro interrupt_pointer_table_entry 1
    dd isr%1
%endmacro
interrupt_pointer_table_entries:
%assign i 0
%rep 512
    interrupt_pointer_table_entry i
%assign i i+1
%endrep