;#########################################################;
;######################CALLABLE METHOD####################;
;#########################################################;

clear_keybard_buffer:
	pushad
	mov edi, dword [0x90014]
	xor ecx, ecx
	mov cx, word [0x90010]
	clear_keybard_buffer_loop:
		mov byte [edi], 0
		inc edi
		loop clear_keybard_buffer_loop
	popad
	ret

disable_keyboard:
	and byte [0x90018], 11110111b
	ret

enable_keyboard:
	or byte [0x90018], 0000100b
	ret



getchar: ;al=char chr
	push ebx
	push ecx
	push edx
	mov cx, word [0x90012]
	cmp cx, 0
	je getchar_nochar
	mov ebx, dword [0x90012]
	and ebx, 0x0000ffff
	mov edi, dword [0x90014]
	xor eax, eax
	mov al, byte [edi+ebx]
	mov byte [edi+ebx], 0
	dec cx
	mov word [0x90012], cx
	mov ebx, key_mapping
	mov dl, byte [0x90018]
	and dl, 10000000b
	cmp dl, 10000000b
	je getchar_caps
	mov dl, byte [0x90018]
	and dl, 01000000b
	cmp dl, 01000000b
	je getchar_caps
	getchar_next:
	add ebx, eax
	dec ebx
	mov eax, [ebx]
	pop edx
	pop ecx
	pop ebx
	ret
	getchar_nochar:
		mov al, 0
		pop edx
		pop ecx
		pop ebx
		ret
	getchar_caps:
		mov ebx, key_mapping_caps
		jmp getchar_next


key_mapping:
	db 0, '1234567890-=', 0, 0, 'qwertyuiop[]', 0, 0, 'asdfghjkl;', 0x27, '`', 0, '\zxcvbnm,./', 0, '*', 0, ' ', 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
	db '789-456+1230.', 0, 0

key_mapping_caps:
	db 0, '!@#$%^&*()_+', 0, 0, 'QWERTYUIOP{}', 0, 0, 'ASDFGHJKL:', 0x22, '~', 0, '|ZXCVBNM<>?', 0, '*', 0, ' ', 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
	db '789-456+1230.', 0, 0

	


;#########################################################;
;######################INTERAL METHOD#####################;
;#########################################################;




init_keyboard_buffer:
	; free config space 0x90010-0x91000
	; struct keyboard_buffer{
	;		word size  ->  0x90010 default 0x400
	;		word used_size  ->  0x90012
	;		ptr begin	->  0x90014
	;
	;		byte LED_status  -> 0x90018    CAPS_LOCK, SHIFT, CTRL, ALT, ENABLE, RESERVED, RESERVED, RESERVED
	;}
	; use config size 8-bytes
	; buffer addr 0x91000-0x91400
	mov word [0x90010], 0x10
	mov word [0x90012], 0
	mov dword [0x90014], 0x00091000
	mov byte [0x90018], 00001000b
	ret


keyboard_int:
	cli
	pushad
	push ds
	push es
	push fs
	push gs
	push ss

	xor eax, eax

	mov al, 0x01
	out 0x21, al
	in al, 0x60

	mov dl, byte [0x90018]
	and dl, 00001000b
	cmp dl, 00001000b
	jne keyboard_int_fin


	cmp al, 0x1d
	je keyboard_int_ctrl
	cmp al, 0x2a
	je keyboard_int_shift
	cmp al, 0x3a
	je keyboard_int_caps
	cmp al, 0x38
	je keyboard_int_alt
	cmp al, 0x9d
	je keyboard_int_ctrl_rea
	cmp al, 0xaa
	je keyboard_int_shift_rea
	cmp al, 0xb8
	je keyboard_int_alt_rea


	cmp al, 0x58
	jbe  keyboard_int_process
	; send EOI msg to hardware

	keyboard_int_fin:
	mov al, 0x20
	out 0x20, al
	out 0xa0, al

	pop ss
	pop gs
	pop fs
	pop es
	pop ds
	popad
	iret

	keyboard_int_process:
			mov ebx, dword [0x90012]
			inc ebx
			and ebx, 0x0000ffff
			mov edx, dword [0x90010]
			and edx, 0x0000ffff
			cmp ebx, edx
			jg keyboard_int_process_error

			mov edi, dword [0x90014]
			mov [edi+ebx], al
			inc word [0x90012]

			jmp keyboard_int_fin

	keyboard_int_process_error:
		mov esi, keyboard_errmsg
		call printk

		jmp keyboard_int_fin


	keyboard_int_ctrl:
		or byte [0x90018], 00100000b
		jmp keyboard_int_fin

	keyboard_int_shift:
		or byte [0x90018], 01000000b
		jmp keyboard_int_fin

	keyboard_int_caps:
		xor byte [0x90018], 10000000b
		jmp keyboard_int_fin

	keyboard_int_alt:
		or byte [0x90018], 00010000b
		jmp keyboard_int_fin



	keyboard_int_ctrl_rea:
		and byte [0x90018], 11011111b
		jmp keyboard_int_fin

	keyboard_int_shift_rea:
		and byte [0x90018], 10111111b
		jmp keyboard_int_fin

	keyboard_int_alt_rea:
		and byte [0x90018], 11101111b
		jmp keyboard_int_fin



keyboard_errmsg db 'keyboard: buffer is full', 0x0d, 0