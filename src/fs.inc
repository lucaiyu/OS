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




spaces db '  ', 0