general_err:
	cli
	pushad
	push ds
	push es
	push fs
	push gs
	push ss

	mov esi, generrmsg
	call printk

	jmp $

	pop ss
	pop gs
	pop fs
	pop es
	pop ds

	iret

generrmsg db 'segmentation fault', 0x0d, 0