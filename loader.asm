[ORG 0x7c00]


mov ah, 0x00
mov al, 0x03
int 0x10

mov si, msg
call print

mov dh, 1
call disk

mov si, 0x8000
call print

jmp $


print:
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

disk:
	pusha
	push dx
	
	mov cx, 0x8000
	mov es, cx
	mov bx, 0
	mov ah, 0x02
	mov al, dh
	mov cl, 0x02
	mov ch, 0x00
	mov dh, 0x00
	int 0x13
	jc error
	pop dx
	cmp al, dh
	jne error
	popa
	ret
error:
	mov si, de
	call print
	jmp $
	ret

msg: db 'loading kernel on sector 2', 0
de: db'DISK READ ERROR IN 0:0:2', 0

times 510-($-$$) db 0
db 0x55,0xaa