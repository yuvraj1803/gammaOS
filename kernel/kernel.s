[BITS 32]

CODE_SEG equ 0x08
DATA_SEG equ 0x10

global _start
global set_all_segments_to_kernel_data_segment

extern kernel_start

_start:
	; init memory segments
	mov ax, DATA_SEG
	mov ds, ax
	mov ss, ax
	mov es, ax
	mov fs, ax
	mov gs, ax
	
	mov ebp, 0x00200000
	mov esp, ebp

	call enable_a20
	call pic_init
	
	; call the kernel!
	call kernel_start

	jmp $

; enables a20 line
enable_a20:
	in al, 0x92
	or al, 2
	out 0x92, al


	ret

; initialises programmable interrupt controller
pic_init:

	; remap master pic
	mov al, 0x11
	out 0x20, al

	; 0x20 is where master isr will start
	mov al, 0x20	
	out 0x21,al

	; end remap of master pic
	mov al, 0x1
	out 0x21, al

	ret
	
; aligns into sector 1
times 512 - ($ - $$) db 0

set_all_segments_to_kernel_data_segment:
	mov ax, 0x10
	mov ds, ax
	mov es, ax
	mov fs, ax
	mov gs, ax

	ret