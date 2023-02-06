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
	
rdisk: ; al=secnum, cl=sector, ch=cylinder, dh=header
	pusha
	mov bl, al
	
	mov ah, 0x02
	int 0x13
	jc rerror
	cmp al, bl
	jne rerror
	popa
	ret
rerror:
	mov si, de
	call print
	jmp $
	ret
	
wdisk: ; al=secnum, cl=sector, ch=cylinder, dh=header
	pusha
	mov bl, al
	
	mov ah, 0x03
	int 0x13
	jc werror
	cmp al, bl
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


rtest:
	mov bx, 0x1000
	mov dh, 19
	mov cl, 0x01
	call rdisk
	call rtest