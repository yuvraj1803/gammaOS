[BITS 32]

section .asm

global exec:function

exec:
    push ebp
    mov ebp, esp

    push dword[ebp+8] ; filename
    mov eax, 5
    int 0x80

    add esp, 4
    pop ebp
    ret