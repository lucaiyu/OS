[org 0x7c00]
[bits 16]
jmp entry
; FAT12 format
db 'data    '
dw 512
db 1
dw 1
db 2
dw 224
dw 2880
db 0xf0
dw 9
dw 18
dw 2
dd 0
dd 2880
db 0x00, 0x00, 0x29
dd 0xffffffff
db 'data    '
db 'FAT12   '

entry:
mov si, errmsg
call print
jmp $

print: ;si=char *str
	pusha
	jmp start
start:
	mov al, [si]
	cmp al, 0
	je fin
	mov ah, 0x0e
	int 0x10
	inc si
	jmp start
fin:
	popa
	ret



errmsg db 'this is a data disc! not a bootable disc!', 0
times 510-($-$$) db 0x00
dw 0xaa55