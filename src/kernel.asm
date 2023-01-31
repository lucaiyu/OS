[ORG 0x8000]
[bits 16]
main:
	call printnl
	;call testwrite
	mov si, testfile
	call createfile
	call tty
	hlt

	
	
testfile db 'kernel img'
%include "src/io.inc"
%include "src/tty.inc"
%include "src/fs.inc"
times 4096-($-$$) db 0