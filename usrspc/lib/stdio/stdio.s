[BITS 32]

section .asm

global puts:function
global putch: function

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