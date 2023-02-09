;#########################################################;
;######################CALLABLE METHOD####################;
;#########################################################;



getchar: ;al=char chr
	push ebx
	push ecx
	mov cx, word [0x90012]
	cmp cx, 0
	je getchar_nochar
	mov ebx, dword [0x90012]
	and ebx, 0x0000ffff
	mov edi, dword [0x90014]
	mov al, byte [edi+ebx]
	mov byte [edi+ebx], 0
	dec cx
	mov word [0x90012], cx
	pop ecx
	pop ebx
	ret
	getchar_nochar:
		mov al, 0
		pop ecx
		pop ebx
		ret


key_mapping:
	

	


;#########################################################;
;######################INTERAL METHOD#####################;
;#########################################################;




init_keyboard_buffer:
	; free config space 0x90010-0x91000
	; struct keyboard_buffer{
	;		word size  ->  0x90010 default 0x400
	;		word used_size  ->  0x90012
	;		ptr begin	->  0x90014
	;}
	; use config size 8-bytes
	; buffer addr 0x91000-0x91400
	mov word [0x90010], 0x400
	mov word [0x90012], 0
	mov dword [0x90014], 0x00091000
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

	cmp al, 0x58
	jbe  keyboard_int_process
	; send EOI msg to hardware


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
			xchg bx, bx
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
	keyboard_int_process_error:
		mov esi, keyboard_errmsg
		call printk

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



keyboard_int_msg db 'keyboard int %', 0x0d, 0
keyboard_errmsg db 'keyboard: buffer is full', 0x0d, 0