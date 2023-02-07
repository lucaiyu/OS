entry:
	mov ah, 0x0e
	mov al, 'S'
	int 0x10
	jmp $

times 2048-($-$$) db 0