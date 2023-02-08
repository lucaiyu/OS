%include "boot/config.inc"

INITSEG EQU DEF_INITSEG
SYSSEG EQU DEF_SYSSEG
SETUPSEG EQU DEF_SETUPSEG

jmp entry


entry:
	mov ax, SETUPSEG
	mov ds, ax
	mov si, setupmsg
	call print
	call nl

	mov ax, INITSEG
	mov es, ax

	mov ah, 0x88
	int 0x15
	mov [es:2], ax

	mov ah, 0x12
	mov bl, 0x10
	int 0x10
	mov [es:8], ax
	mov [es:10], bx
	mov [es:12], cx

	mov ax, 0x5019
	mov [es:14], ax

	mov ah, 0x03
	xor bh, bh
	int 0x10
	mov [es:0], dx

	push ds
	mov ax, 0
	mov ds, ax
	lds si, [4*0x41]
	mov ax, INITSEG
	mov es, ax
	mov di, 0x0080
	mov cx, 0x10
	rep
	movsb

	lds si, [4*0x46]
	mov ax, INITSEG
	mov es, ax
	mov di, 0x0090
	mov cx, 0x10
	rep
	movsb
	pop ds

	mov	ax, 0x01500
	mov	dl, 0x81
	int	0x13
	jc no_disk1
	cmp	ah, 3
	je is_disk1
no_disk1:
	mov	ax, INITSEG
	mov	es, ax
	mov	di, 0x0090
	mov	cx, 0x10
	mov	ax, 0x00
	rep
	stosb
is_disk1:
	
	mov si, getmsg
	call print
	call nl

	mov cx, 19
	scoll:
		call nl
		loop scoll

	mov bh, 0
	mov dh, 5
	mov dl, 0
	mov ah, 0x02
	int 0x10
	

	; mov ah, 0x01
	; mov cx, 0x2607
	; int 0x10


	cli ; WARNING AFTER THIS LINE BIOS INT WILL DIE!!!!!

	call move_system

	mov ax, SETUPSEG
	mov ds, ax

	lidt [idt_48]
	lgdt [gdt_48]


	call empty_8042

	; OPEN A20

	mov al, 0xd1
	out 0x64, al
	call empty_8042
	mov al, 0xdf
	out 0x64, al
	call empty_8042


	; this will get a error to test system movement
	;mov si, getmsg
	;call print
	;call nl

	call set_8259A

	mov eax, cr0
	or eax, 0x1
	mov cr0, eax



	jmp dword 1*8:0


empty_8042:
	dw 0x00eb, 0x00eb
	in al, 0x64
	test al, 2
	jnz empty_8042
	ret

set_8259A: 	
    mov	al,0x11
	out	0x20,al
	dw	0x00eb,0x00eb
	out	0xA0,al
	dw	0x00eb,0x00eb
	mov	al,0x20
	out	0x21,al
	dw	0x00eb,0x00eb
	mov	al,0x28
	out	0xA1,al
	dw	0x00eb,0x00eb
	mov	al,0x04
	out	0x21,al
	dw	0x00eb,0x00eb
	mov	al,0x02
	out	0xA1,al
	dw	0x00eb,0x00eb
	mov	al,0x01
	out	0x21,al
	dw	0x00eb,0x00eb
	out	0xA1,al
	dw	0x00eb,0x00eb
	mov	al,0xFF
	out	0x21,al
	dw	0x00eb,0x00eb
	out	0xA1,al
    ret


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

move_system:
    mov	ax, 0x0000
	cld
do_move:
	mov	es, ax
	add	ax, 0x1000
	cmp	ax, 0x9000
	jz end_move
	mov	ds, ax
	sub	di, di
	sub	si, si
	mov cx, 0x8000
	rep
	movsw
	jmp	do_move
end_move:
	ret


setupmsg db 'loading 32-bit mode', 0
getmsg db 'getting system profile', 0


idt_48:
	dw 0x800
	dw 0, 0
gdt_48:
	dw 0x800
	dw 512+gdt, 0x9

gdt:
	dw 0,0,0,0

	dw 0x07ff
	dw 0
	dw 0x9a00 ; R-w-X
	dw 0x00c0

	dw 0x07ff
	dw 0x0
	dw 0x9200 ; R-W-x
	dw 0x00c0


times 2048-($-$$) db 0