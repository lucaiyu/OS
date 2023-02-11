%include "kernel/console.inc"
%include "kernel/int.inc"
%include "kernel/keyboard.inc"
%include "kernel/ata.inc"

[bits 32]
main:
	call init

	push dword [0x100000]
	push dword [0x100004]
	push dword [0x100008]
	push dword [0x10000b]
	mov esi, atamsg
	call printk

	mov cl, 1
	mov ebx, 0
	mov edi, 0x100000
	call ata_read

	push dword [0x100000]
	push dword [0x100004]
	push dword [0x100008]
	push dword [0x10000b]
	mov esi, atamsg
	call printk

	jmp $


init:
	pushad
	mov byte [0x90000], 0x00
	mov byte [0x90000+1], 0x0a

	call init_idt
	call open_keyboard
	call init_keyboard_buffer

	sti

	mov esi, initmsg
	call printk
	popad
	ret

initmsg db 'all devices inited', 0x0d, 0
atamsg db 'ata read % % % %', 0x0d, 0