%include "kernel/console.inc"
%include "kernel/int.inc"
%include "kernel/keyboard.inc"

[bits 32]
main:
	call init

	sti
	getloop:
		call getchar
		cmp al, 0
		je getfail
		call putchar
		getfail:
			jmp getloop

	jmp $


init:
	pushad
	mov byte [0x90000], 0x00
	mov byte [0x90000+1], 0x0a

	call init_idt
	call enable_keyboard
	call init_keyboard_buffer

	sti

	mov esi, initmsg
	call printk
	popad
	ret

initmsg db 'all devices inited', 0x0d, 0