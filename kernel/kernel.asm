%include "kernel/console.inc"

[bits 32]
main:
	call init
	mov esi, initmsg
	call printk

	jmp $


init:
	pushad
	; init cursor
	mov byte [0x90000], 0x00
	mov byte [0x90000+1], 0x0a
	popad
	ret

initmsg db 'all things inited', 0x0d, 0