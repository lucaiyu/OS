%include "kernel/console.inc"
%include "kernel/int.inc"
%include "kernel/keyboard.inc"
%include "kernel/ata.inc"

[bits 32]
main:
	call init

	mov eax, 1
	mov cl, 10
	mov edi, 0xa0000
	call ata_read

	jmp $


init:
	pushad
	mov byte [0x90000], 0x00
	mov byte [0x90000+1], 0x0a

	call init_idt
	call enable_keyboard
	call init_keyboard_buffer

	;sti

	mov esi, initmsg
	call printk
	popad
	ret

initmsg db 'all devices inited', 0x0d, 0