init_idt:
	ret
	; register idt

	lea edx, [keyboard_int]
	mov eax,0x00080000
	mov ax, dx
	mov dx,0x8E00
	lea edi, [_idt]
	add edi, 8*21
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
	sti
	ret

enable_keyboard:
	mov al, 11111101b
	out 021h, al
	dw 0x00eb,0x00eb
	mov al, 11111111b
	out 0A1h, al
	dw	0x00eb,0x00eb
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

	push eax
	mov esi, keyboard_int_msg
	call printk
	add esp, 4

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



keyboard_int_msg db 'keyboard int scancode: % ', 0x0d, 0