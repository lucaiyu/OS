putchar: ;al=char c
	xchg bx, bx
	pushad
	mov ebx, dword [0x90000]
	and ebx, 0x000000FF
	mov ecx, dword [0x90000+1]
	and ecx, 0x000000FF
	mov edi, 0xb8000
	push eax
	mov eax, 160
	mul ecx
	mov ecx, eax
	mov eax, 2
	mul ebx
	mov ebx, eax
	add ebx, ecx
	add edi, ebx
	pop eax
	mov byte [edi], al
	mov byte [edi+1], 0x0f
	; move cursor
	sub edi, 0xb8000
	shr edi, 1
	inc edi
	mov bx, di
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

	; rewrite config
	mov ebx, dword [0x90000]
	and ebx, 0x000000FF
	mov ecx, dword [0x90000+1]
	and ecx, 0x000000FF
	inc ebx
	cmp ebx, 80
	jg .interal_nl
	inc byte [0x90000]

	popad
	ret
		.interal_nl:
		cmp ecx, 25
		jge .interal_putchar_scoll
		mov byte [0x90000], 0
		inc byte [0x90001]
		popad
		ret

		.interal_putchar_scoll:
			call scoll
			popad
			ret

scoll:
	pushad
	mov edi, 0xb8000
	mov esi, 0xb8000+160
	.interal_scoll:
		mov edx, dword [esi]
		mov dword [edi], edx
		add edi, 4
		add esi, 4
		cmp esi, 0xb8000+160*25
		jge .interal_scoll_fin
		jmp .interal_scoll
	.interal_scoll_fin:
		; revert cursor
		mov byte [0x90000], 0
		mov ebx, dword [0x90001]
		and ebx, 0x000000FF
		mov ax, bx
		mov dl, 80
		mul dl
		xor bx, bx
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


printk: ; esi=char *str
	pushad
	.interal_printk_loop
	mov al, byte [esi]
	; is end?
	cmp al, 0
	je .interal_printk_fin
	; is enter?
	cmp al, 0x0d
	je .interal_printk_nl
	call putchar
	inc esi
	jmp .interal_printk_loop

	.interal_printk_fin:
		popad
		ret

.interal_printk_nl:
	pushad
	mov byte [0x90000], 0
	inc byte [0x90001]
	mov ebx, dword [0x90001]
	and ebx, 0x000000FF
	mov ax, bx
	mov dl, 80
	mul dl
	xor bx, bx
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
	inc esi
	jmp .interal_printk_loop