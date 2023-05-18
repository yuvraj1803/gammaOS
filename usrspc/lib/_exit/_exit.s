[BITS 32]

section .asm

global _exit

_exit:

    push ebp
    mov ebp, esp

    mov eax, 6 ; call __0x80_EXIT system call handler
    int 0x80

    pop ebp
    ret