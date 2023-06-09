 
 ;
 ;
 ;
 ;
 ;
 ; Copyright (c) 2023 Yuvraj Sakshith <ysakshith@gmail.com>
 ;
 ;
 ;
 ;
 ;
 ;


ORG 0x7C00
BITS 16

CODE_SGM equ gdt_code - gdt_start
DATA_SGM equ gdt_data - gdt_start

; Boot Parameter Block
jmp short start
nop

OEMIdentifier           db 'gammaOS '
BytesPerSector          dw  0x200
SectorsPerCluster       db  0x80
ReservedSectors         dw  200
NumberOfFATs            db  0x02
RootEntries             dw  0x40
SectorsInVolume         dw  0x00
MediaDESCType           db  0xF8
SectorsPerFAT           dw  0x100
SectorsPerTrack         dw  0x20
HeadsInMedia            dw  0x40
HiddenSectors           dd  0x00
LargeSectors            dd  0x773594

; Extended Boot Parameter Block
DriveNumber             db  0x80
WinNTFlags              db  0x00
Signature               db  0x29
VolumeID                dd  0xD105
VolumeLabel             db 'gamma BOOT '
SystemID                db 'FAT16   '


times 33 db 0

start:
	jmp 0:seg_init

; Initialise memory segments in Real Mode
seg_init:
	cli	
	mov ax,0x00
	mov ds,ax
	mov es,ax
	mov ss,ax
	mov sp,0x7c00

	sti

.load_protected:
	cli
	lgdt[gdt_descriptor]
	mov eax, cr0
	or  eax, 0x1
	mov cr0, eax


	jmp CODE_SGM:load32

; Setting up global descriptor table

gdt_start:
gdt_null:
	dd 0x0
	dd 0x0

gdt_code:
	dw 0xffff
	dw 0
	db 0
	db 0x9a
	db 11001111b
	db 0

gdt_data:
	dw 0xffff
	dw 0
	db 0
	db 0x92
	db 11001111b
	db 0

gdt_end:

gdt_descriptor:
	dw gdt_end - gdt_start - 1 
	dd gdt_start


;Protected mode begins here
; Kernel code begins at 0x100000
 [BITS 32]
 load32:
    mov eax, 1
    mov ecx, 100
    mov edi, 0x0100000
    call ata_lba_read
    jmp CODE_SGM:0x0100000

ata_lba_read:
    mov ebx, eax, ; Backup the LBA
    ; Send the highest 8 bits of the lba to hard disk controller
    shr eax, 24
    or eax, 0xE0 ; Select the  master drive
    mov dx, 0x1F6
    out dx, al
    ; Finished sending the highest 8 bits of the lba

    ; Send the total sectors to read
    mov eax, ecx
    mov dx, 0x1F2
    out dx, al
    ; Finished sending the total sectors to read

    ; Send more bits of the LBA
    mov eax, ebx ; Restore the backup LBA
    mov dx, 0x1F3
    out dx, al
    ; Finished sending more bits of the LBA

    ; Send more bits of the LBA
    mov dx, 0x1F4
    mov eax, ebx ; Restore the backup LBA
    shr eax, 8
    out dx, al
    ; Finished sending more bits of the LBA

    ; Send upper 16 bits of the LBA
    mov dx, 0x1F5
    mov eax, ebx ; Restore the backup LBA
    shr eax, 16
    out dx, al
    ; Finished sending upper 16 bits of the LBA

    mov dx, 0x1f7
    mov al, 0x20
    out dx, al

    ; Read all sectors into memory
.next_sector:
    push ecx

; Checking if we need to read
.try_again:
    mov dx, 0x1f7
    in al, dx
    test al, 8
    jz .try_again

; We need to read 256 words at a time
    mov ecx, 256
    mov dx, 0x1F0
    rep insw
    pop ecx
    loop .next_sector
    ; End of reading sectors into memory
    ret

; fill first sector with nulls
times 510-($ - $$) db 0
; Boot signature 0x55AA written in little-endian format
dw 0xAA55