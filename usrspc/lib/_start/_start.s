[BITS 32]

section .asm

global _start
extern main
extern _exit

_start:

    call main
    call _exit
    ret
