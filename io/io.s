section .asm

global insb
global insw
global insdw
global outb
global outw
global outdw

; read a byte from provided port
insb:
    push ebp
    mov ebp, esp

    xor eax, eax

    mov edx, [ebp + 8]
    in al, dx

    pop ebp
    ret

; read a word from provided port
insw: 

    push ebp
    mov ebp, esp

    xor eax, eax

    mov edx, [ebp + 8]
    in ax, dx

    pop ebp
    ret

; read a double-word(32-bit) from provided port

insdw:

    push ebp
    mov ebp, esp

    xor eax, eax

    mov edx, [ebp + 8]
    in eax, dx

    pop ebp
    ret


; send a byte to provided port
outb:
    push ebp
    mov ebp, esp

    xor eax, eax

    mov eax, [ebp + 12]
    mov edx, [ebp + 8]

    out dx, al

    pop ebp
    ret

; send a word to provided port
outw:

    push ebp
    mov ebp, esp

    xor eax, eax

    mov eax, [ebp + 12]
    mov edx, [ebp + 8]

    out dx, ax

    pop ebp
    ret

; send a double-word (32-bit) to provided port
outdw:
    
    push ebp
    mov ebp, esp

    xor eax, eax

    mov eax, [ebp + 12]
    mov edx, [ebp + 8]

    out dx, eax

    pop ebp
    ret

