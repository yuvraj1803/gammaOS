section .asm

global tss_load

tss_load:
    mov ax, [esp+4] ; tss base
    ltr ax ; load task register

    ret

