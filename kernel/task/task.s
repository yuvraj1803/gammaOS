[BITS 32]

global restore_gprs
global enter_task
global set_all_segments_to_user_data_segment

enter_task:
    mov ebp, esp

    mov ebx, [ebp+4]

    push dword [ebx+40] ; ss
    
    push dword [ebx+24] ; esp

    pushf ; flags
    pop eax
    or eax, 0x200
    push eax

    push dword [ebx+36] ; cs

    push dword [ebx+32] ; ip

    mov ax, [ebx+40] ; ss
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax

    push dword [ebp+4] ; push struct reg* registers base address
    call restore_gprs
    add esp, 4

    iretd


restore_gprs:

    push ebp
    mov ebp, esp

    mov eax, [ebp + 8] ;; we will be losing track of ebp/esp in sometime, we can use eax as a temporary variable,
                        ; to store the base address of registers structure passed.
    
    mov ebx, [eax+4]
    mov ecx, [eax+8]
    mov edx, [eax+12]
    mov esi, [eax+16]
    mov edi, [eax+20]

    mov ebp, [eax+28]
 
    mov eax, [eax] ;; finally restoring eax

    add esp, 4

    ret


set_all_segments_to_user_data_segment:
    mov ax, 0x23
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    ret


    