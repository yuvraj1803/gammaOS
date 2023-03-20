[BITS 32]

section .asm

global load_pd_to_cr3
global enable_paging

load_pd_to_cr3:
    push ebp
    mov  ebp, esp

    mov eax, [ebp + 8] ; move the argument to eax. the argument was the address of page directory btw.
    mov cr3, eax

    pop ebp
    ret


enable_paging:
    push ebp
    mov ebp, esp

    mov eax, cr0
    or  eax, 0x80000000 ; msb of cr0 must be set to enable paging.
    mov cr0, eax

    pop ebp
    ret