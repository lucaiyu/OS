%include "src/boot/config.inc"

jmp entry

SETUPLEN equ 4
BOOTSEG equ 0x07c0
INITSEG equ	DEF_INITSEG
SETUPSEG equ DEF_SETUPSEG
SYSSEG equ DEF_SYSSEG

SETUPSector equ 2
SYSSector equ SETUPSector+SETUPLEN
SYScylind equ 7

ROOT_DEV equ 0
SWAP_DEV equ 0

entry:
	mov ax, 0
	mov ss, ax
	mov sp, BOOTSEG

	mov ax, BOOTSEG
	mov ds, ax
	mov si, bootmsg
	call nl
	call print
	call nl

	mov ax, INITSEG
	mov es, ax
	mov cx, 256
	sub si, si
	sub di, di
	rep
	movsw
	jmp INITSEG:go

go:
	mov ax, cs
	mov ds, ax
	mov si, movemsg
	call print
	call nl

	mov ax, cs
	mov ss, ax
	mov sp, 0xfef4

	mov si, loadmsg
	call print
	call nl

	mov ax, SETUPSEG
	mov es, ax
	mov cl, SETUPSector

	call loadsetup


	mov si, kernmsg
	call print
	call nl

	mov ax, SYSSEG
	mov es, ax
	mov cl, SYSSector
	call loadsystem

	jmp $




print:
	mov al, [si]
	cmp al, 0
	je finprint
	mov ah, 0x0e
	int 0x10
	inc si
	jmp print
	finprint:
		ret

nl:
	mov ah,0eh
    mov al,0dh
    int 10h
    mov al,0ah
    int 10h
    ret


readsector: ;ch=cylinder dh=header cl=sector
	mov ah, 0x02
	mov al, 1
	mov bx, 0
	mov dl, 0
	int 0x13
	jnc readsecc
	mov si, rerrmsg
	call print
	jmp $
	readsecc:
		ret



loadsetup:
	call readsector
	mov ax, es
	add ax, 0x0020
	mov es, ax
	inc cl
	cmp cl, SETUPLEN+1+1
	jne loadsetup
	ret


loadsystem:
	

bootmsg db 'loading system...', 0
movemsg db 'moved bootsect to 0x9000', 0
loadmsg db 'loading setup program to 0x9020', 0
rerrmsg db 'failed to load setup program: bad program or bad disk', 0
kernmsg db 'loading kernel program to 0x1000', 0

times 512-2*3-($-$$) db 0

swap_dev dw SWAP_DEV
root_dev dw ROOT_DEV

dw 0xaa55