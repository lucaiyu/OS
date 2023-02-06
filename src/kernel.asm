[ORG 0x8000]
[bits 16]
main:
	call printnl

	mov cl, 0x02
	mov al, 0x09
	mov bx, 0x1000
	call rdisk
	mov si, 0x1000
	call print
	;call tty
	jmp $

	
%include "src/io.inc"
%include "src/tty.inc"
%include "src/fs.inc"
times 4096-($-$$) db 0