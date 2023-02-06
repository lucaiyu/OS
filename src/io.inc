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
	
	
printnl: ;void
	pusha
	mov ah, 0x0e
	mov al, 0x0a
	int 0x10
	mov al, 0x0d
	int 0x10
	popa
	ret
	
rdisk:
	pusha
	push dx
	
	mov ah, 0x02
	mov al, dh
	mov cl, cl
	mov ch, 0x00
	mov dh, 0x00
	mov dl, 0x01
	int 0x13
	jc rerror
	pop dx
	cmp al, dh
	jne rerror
	popa
	ret
rerror:
	mov si, de
	call print
	jmp $
	ret
	
wdisk:
	pusha
	push dx
	
	mov ah, 0x03
	mov al, dh
	mov cl, cl
	mov ch, 0x00
	mov dh, 0x00
	mov dl, 0x01
	int 0x13
	jc werror
	pop dx
	cmp al, dh
	jne werror
	popa
	ret
werror:
	mov si, wde
	call print
	jmp $
	ret
de: db 'DISK READ ERROR', 0
wde: db 'DISK WRITE ERROR', 0