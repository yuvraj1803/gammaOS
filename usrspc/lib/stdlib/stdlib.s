[BITS 32]

section .asm

global malloc:function
global free:function

malloc:

    push ebp
    mov ebp, esp

    push dword[esp + 8]
    mov eax, 3 ; syscall 3 => malloc
    int 0x80
    add esp, 4

    pop ebp
    ret

free:
    push ebp
    mov ebp, esp

    push dword[esp + 8]
    mov eax, 4 ; syscall 4 => free

    int 0x80
    add esp, 4

    pop ebp
    ret
