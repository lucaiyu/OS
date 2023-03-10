ata_read: ; cl=sector_num ebx=LBA edi=buffer
	pushad
	mov dx, 0x1f2
	mov al, cl
	out dx, al

	mov dx, 0x1f3
	mov al, bl
	out dx, al

	mov dx, 0x1f4
	mov al, bh
	out dx, al

	shr ebx, 16

	mov dx, 0x1f5
	mov al, bl
	out dx, al

	mov dx, 0x1f6
	mov al, bh
	and al, 0x0f
	or al, 0xe0
	out dx, al

	mov dx, 0x1f7
	mov al, 0x20
	out dx, al

	mov dx, 0x1f7

	wait_read_busy:
		nop
		in al, dx
		and al, 0x88
		cmp al, 0x08
		jnz wait_read_busy

	and ecx, 0x000000ff
	mov ax, 256
	mul cx
	mov cx, ax

	mov dx, 0x1f0
	read_loop:

		in ax, dx
		mov [edi], ax
		add edi, 2
		loop read_loop


	popad
	ret


ata_write: ; cl=sector_num ebx=LBA esi=buffer
	pushad
	mov dx, 0x1f2
	mov al, cl
	out dx, al

	mov dx, 0x1f3
	mov al, bl
	out dx, al

	mov dx, 0x1f4
	mov al, bh
	out dx, al

	shr ebx, 16

	mov dx, 0x1f5
	mov al, bl
	out dx, al

	mov dx, 0x1f6
	mov al, bh
	and al, 0x0f
	or al, 0xe0
	out dx, al

	mov dx, 0x1f7
	mov al, 0x30
	out dx, al

	mov dx, 0x1f7

	wait_write_busy:
		nop
		in al, dx
		and al, 0x88
		cmp al, 0x08
		jnz wait_write_busy

	and ecx, 0x000000ff
	mov ax, 256
	mul cx
	mov cx, ax

	mov dx, 0x1f0
	write_loop:
		mov ax, [esi]
		out dx, ax
		add esi, 2
		loop write_loop


	popad
	ret


