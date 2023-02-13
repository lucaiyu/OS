init_idt:
	; register idt
	lea edx, [keyboard_int]
	mov ecx, 0x21
	call register_int_service

	mov ecx, 0x15
	lea edx, [general_err]
	regerr:
		call register_int_service
		loop regerr

	lidt [idt_descr]

	; enable IRQ
	mov al, 11111101b
	out 021h, al
	dw 0x00eb,0x00eb
	mov al, 11111111b
	out 0A1h, al
	dw	0x00eb,0x00eb
	ret

register_int_service: ; edx=addr of service, ecx=int num
	pushad

	push edx

	mov eax, 8
	mul ecx
	mov ecx, eax

	pop edx

	mov eax,0x00080000
	mov ax, dx
	mov dx,0x8E00
	lea edi, [_idt]

	add edi, ecx
	mov [edi], eax
	mov [edi+4], edx
	popad
	ret

open_keyboard:
	mov al, 11111101b
	out 021h, al
	dw 0x00eb,0x00eb
	mov al, 11111111b
	out 0A1h, al
	dw	0x00eb,0x00eb
	ret