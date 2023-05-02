[BITS 32]

section .asm

global _start

_start:
    mov eax, 0x10
    int 0x80
    jmp $