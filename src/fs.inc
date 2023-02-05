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
		cmp bx, 0x0a
		jge firstfin
		jmp first

	firstfin:
		popa
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
		cmp bx, 0x9000+4096
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
		pusha
		inc cx
		mov word [bx], cx
		mov ax, 20
		mul cx
		mov word [bx+2], ax
		add ax, 20
		mov word [bx+4], ax
		mov ax, 0
		add bx, 6
		strcpy:
			mov dl, [si]
			mov [bx], dl
			inc bx
			inc si
			inc ax
			cmp ax, 10
			jge cpyend
			jmp strcpy
		cpyend:
			nop
		popa
		mov dh, 0x01
		mov cl, 0x0a
		mov bx, 0x9000
		call wdisk
		popa
		ret



spaces db '  ', 0
toucherrmsg db 'unable to touch file: filesystem table is full', 0