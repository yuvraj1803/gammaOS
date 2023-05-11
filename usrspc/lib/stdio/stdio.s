[BITS 32]

section .asm

global puts:  function
global putch: function

global gets:  function
global getch: function
puts:
    push ebp
    mov ebp, esp

    push dword[ebp+8]
    mov eax, 0
    int 0x80
    
    add esp,4

    pop ebp
    ret

putch:
    push ebp
    mov ebp, esp

    push dword[ebp+8]
    mov eax, 2
    int 0x80

    add esp, 4

    pop ebp
    ret


wait_for_key_press:

    mov eax, 1
    int 0x80
    cmp eax, 0
    je wait_for_key_press

    ret

getch:

    push ebp
    mov ebp, esp

    call wait_for_key_press

    pop ebp
    ret

gets:
    
    ret