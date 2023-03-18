[BITS 32]

CODE_SEG equ 0x08
DATA_SEG equ 0x10

global _start

extern kinit

_start:

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
	
	sti

	call kinit

	jmp $

enable_a20:
	in al, 0x92
	or al, 2
	out 0x92, al

	ret

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
	

times 512 - ($ - $$) db 0
