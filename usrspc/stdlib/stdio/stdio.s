[BITS 32]

global puts:function

puts:
    push ebp
    mov ebp, esp

    push dword[ebp+8]
    mov eax, 1
    int 0x80
    
    add esp,4

    pop ebp
    ret