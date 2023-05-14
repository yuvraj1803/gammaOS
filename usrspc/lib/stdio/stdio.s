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
    push ebp
    mov ebp, esp

    mov ebx, [ebp+8] ; address of the string passed

.loop:
    call getch ; this will take one char input and place it in eax
    mov [ebx], eax
    add ebx, 1
    cmp eax, 10 ; compare it with enter key's ascii
    jne .loop

    mov eax,[ebp+8] ; place address of return string in eax

    pop ebp
    ret