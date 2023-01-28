cls:
	pushad
	mov eax, 0
clear_screen:
	mov byte [gs:eax], ' '
	add eax, 2
	cmp eax, 0x00000f9f
	jge fin
	jmp clear_screen
fin:
	xor ebx, ebx
	call mov_cursor_length
	popad
	ret
	

mov_cursor_length: ;bx=length
	pushad
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

get_cursor_pos: ;bx=pos
	mov dx,03d4h
	mov al,0eh
	out dx,al
	inc dx
	in al,dx
	mov bh,al
 
	dec dx
	mov al,0fh
	out  dx,al
	inc dx
	in al,dx
	mov bl,al
	ret
	

	
printf: ;si=char *str
pushad
xor bx, bx
call get_cursor_pos
shl bx, 1
start_print:
	mov al, byte [si]
	cmp al, 0
	je fini
	mov byte [gs:bx], al
	inc si
	add bx, 2
	jmp start_print
fini:
	shr bx, 1
	cmp bx, 0x0f9f-80
	jg clear
	call mov_cursor_length
	popad
	ret
clear:
	call cls


println: ;void
	pushad
	xor bx, bx
	call get_cursor_pos
	cmp bx, 80
	jb exec
	mod:
		sub bx, 80
		cmp bx, 80
		jb exec
		jmp mod
	exec:
		mov cx, bx
		xor bx, bx
		call get_cursor_pos
		sub bx, cx
		add bx, 80
		call mov_cursor_length
		popad
		ret


putchar: ;al=char
	pushad
	xor bx, bx
	call get_cursor_pos
	shl bx, 1
	mov byte [gs:bx], al
	popad
	ret

intmsg: db 'You pressed key ', 0
getchar:
	mov al,  0x61
	out 0x20, al
	in  al,  0x60

	cmp al, 0x02
	jge int1

	mov al, 0x20
	out 0x20 , al
	out 0xa0 , al

	ret
	int1:
		cmp al, 0x35
		jbe int2
		ret
	int2:
		cmp ecx, 0xfffff
		jb int3
		mov si, intmsg
		call printf
		call putchar
		call println
		mov ecx, 0
	int3:
		inc ecx
		ret
