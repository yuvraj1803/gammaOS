[BITS 32]

section .asm

global _start

_start:
    int 0x80
    jmp $