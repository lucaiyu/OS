listfile:
	pusha
	mov di, 0x9006
	mov si, spaces
	filels:
		cmp word [di-6], 0x0000
		je lsend
		call printfilename
		call print
		add di, 16
		jmp filels
	lsend:
		popa
		ret

printfilename: ;di=char *str
	pusha
	xor bx, bx
	mov ah, 0x0e
	first:
		mov al, byte [di+bx]
		inc bx
		cmp al, ' '
		je firstfin
		cmp al, 0
		je firstfin
		int 0x10
		cmp bx, 0x07
		jge firstfin
		jmp first

	firstfin:
		mov al, '.'
		int 0x10
		mov bx, 0x07

	next:
		mov al, byte [di+bx]
		inc bx
		cmp al, ' '
		je nextfin
		cmp al, 0x00
		je nextfin
		int 0x10
		cmp bx, 0x0a
		jge nextfin
		jmp next

	nextfin:
		popa
		ret



testwrite:
	mov dword [0x9000], 0xffffffff
	call wdisk
	ret

createfile: ;si=char *filename
	pusha
	mov bx, 0x9000
	mov cx, 0
	findend:
		cmp word [bx], 0x0000
		je wintofs
		add bx, 16
		inc cx
		cmp bx, 0x9000+32
		jge createerr
		jmp findend
	createerr:
		pusha
		mov si, toucherrmsg
		call print
		call printnl
		popa
		popa
		ret
	wintofs:
		pushad
		inc cx
		mov word [bx], cx
		mov word [bx+2], 0x0000
		mov word [bx+4], 0x0000
		mov ax, 0
		add bx, 6
		strcpy:
			
		popad
		mov dh, 0x01
		mov cl, 0x0a
		mov bx, 0x9000
		call wdisk
		popa
		ret



spaces db '  ', 0
toucherrmsg db 'unable to touch file: filesystem table is full', 0