init_idt:
	; register idt

	; reister keyboard int service
	lea edx, [keyboard_int]
	mov eax,0x00080000
	mov ax, dx
	mov dx,0x8E00
	lea edi, [_idt]
	add edi, 8*0x21
	mov [edi], eax
	mov [edi+4], edx
	lidt [idt_descr]

	; enable IRQ
	mov al, 11111101b
	out 021h, al
	dw 0x00eb,0x00eb
	mov al, 11111111b
	out 0A1h, al
	dw	0x00eb,0x00eb
	ret

open_keyboard:
	mov al, 11111101b
	out 021h, al
	dw 0x00eb,0x00eb
	mov al, 11111111b
	out 0A1h, al
	dw	0x00eb,0x00eb
	ret