%include "kernel/console.inc"
%include "kernel/int.inc"
%include "kernel/keyboard.inc"
%include "kernel/ata.inc"
%include "kernel/fat16.inc"
[bits 32]
main:
	call init

	mov eax, 1
	call read_file

	mov dword [0x140000], 0xffffffff
	mov dword [0x140004], 0xaaaaaaaa
	mov eax, 1
	call write_file

	mov esi, initmsg
	call printk

	jmp $


init:
	pushad
	mov byte [0x90000], 0x00
	mov byte [0x90000+1], 0x0a

	call init_idt
	call open_keyboard
	call init_keyboard_buffer

	;sti

	call fs_init

	mov esi, initmsg
	call printk
	popad
	ret

initmsg db 'all devices inited', 0x0d, 0