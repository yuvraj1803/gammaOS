section .asm

global tss_load

tss_load:
    mov ax, [esp+8] ; tss base
    ltr ax ; load task register

    ret

