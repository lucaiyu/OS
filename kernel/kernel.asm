%include "kernel/console.inc"
%include "kernel/int.inc"
%include "kernel/keyboard.inc"

[bits 32]
main:
	call init
	mov esi, initmsg
	push 0x33333333
	push 0x22222222
	push 0x11111111
	call printk
	add esp, 12

	;call init_idt
	;call enable_keyboard
	mov ecx, 0x0a
	.l:
		push ecx
		mov esi, testmsg
		call printk
		add esp, 4
		xchg bx, bx
		loop .l
	sti


	jmp $


init:
	pushad
	; init cursor
	mov byte [0x90000], 0x00
	mov byte [0x90000+1], 0x0a
	popad
	ret

initmsg db 'format print test: % % % ', 0x0d, 0
testmsg db 'printk test %', 0x0d, 0