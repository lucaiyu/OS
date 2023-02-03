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
	
disk:
	pusha
	push dx
	
	mov ah, 0x02
	mov al, dh
	mov cl, cl
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
	
de: db'DISK READ ERROR IN CHS:002', 0