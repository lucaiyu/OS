%include "kernel/console.inc"
%include "kernel/int.inc"
%include "kernel/keyboard.inc"
%include "kernel/ata.inc"
%include "kernel/fat16.inc"
[bits 32]
main:
	call init

	mov esi, file1
	call create_file
	mov esi, file2
	call create_file
	mov esi, file3
	call create_file


	mov esi, finmsg
	call printk
	call fs_write
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
finmsg db 'all things done!', 0x0d, 0
file1 db 'kernel.img', 0
file2 db 'user.dat', 0
file3 db 'system.bin', 0