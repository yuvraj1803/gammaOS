[BITS 32]

section .asm

global _start

_start:
    push msg
    mov eax, 0x0
    int 0x80
    add esp,4
    jmp $

section .data
msg: db 'This is being printed from userland!',0