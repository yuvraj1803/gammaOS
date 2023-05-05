[BITS 32]

section .asm

global _start

_start:

_loop:
    call waitforkey
    push eax
    mov eax, 2
    int 0x80
    add esp,4 

    jmp _loop

waitforkey:
    mov eax, 1
    int 0x80
    cmp eax, 0
    je waitforkey

    ret

section .data
msg: db 'This is being printed from userland!',0