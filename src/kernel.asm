[ORG 0x8000]
[bits 16]
main:
	call printnl
	call tty
	hlt

	
%include "src/io.inc"
%include "src/tty.inc"
%include "src/fs.inc"
times 4096-($-$$) db 0