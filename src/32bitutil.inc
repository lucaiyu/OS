cls:
	pushad
	mov eax, 0
clear_screen:
	mov byte [ds:eax], ' '
	add eax, 2
	cmp eax, 0x00000f9f
	jge fin
	jmp clear_screen
fin:
	xor eax, eax
	xor ebx, ebx
	call mov_cursor
	popad
	ret
	
mov_cursor: ;bx=x ax=y
	pushad
	mov dl, 80
	mul dl
	add bx, ax
	mov dx, 0x03D4
	mov al, 0x0F
	out dx, al
	
	inc dl
	mov al, bl
	out dx, al
	
	dec dl
	mov al, 0x0E
	out dx, al
	
	inc dl
	mov al, bh
	out dx, al
	popad
	ret
	
	
putchar: ;al=char bx=pos
	mov byte [bx+0xb800], al
	ret
	
printf: ;si=char *str bx=pos
	mov al, byte [ds:si]
	cmp al, 0
	je fini
	mov byte [bx], al
	add si, 2
	add bx, 2
	jmp printf
fini:
	ret