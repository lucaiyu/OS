[ORG 0x8000]
[bits 16]
main:
	mov si, msg
	call print
	call printnl
	call tty
	hlt

	
	
%include "src/io.inc"
%include "src/tty.inc"
msg: db 'kernel loaded', 0
times 4096-($-$$) db 0