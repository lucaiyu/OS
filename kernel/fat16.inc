FAT_ADDR equ 0x100000
ROOT_ADDR equ 0x120000
DATA_ADDR equ 0x140000



fs_init:
	pushad
	mov cl, 128
	mov ebx, 1280
	mov edi, 0x100000
	call ata_read

	mov cl, 128
	mov ebx, 1408
	mov edi, 0x120000
	call ata_read

	popad
	ret

fs_write:
	pushad
	mov cl, 128
	mov ebx, 1280
	mov edi, 0x100000
	call ata_write

	mov cl, 128
	mov ebx, 1408
	mov edi, 0x120000
	call ata_write

	popad
	ret


read_file: ; eax=cluster num
	pushad
	mov esi, 0x100000
	mov ebx, 2
	mul ebx
	add esi, eax
	mov edi, 0x140000
	cluster_read_loop:
		mov bx, word [esi]
		push bx
		cmp bx, 0xfff0
		je cluster_read_error

		; read sector
		mov cl, 1
		mov ebx, eax
		add ebx, 1536
		mov edi, 0x140000
		call ata_read

		pop bx

		cmp bx, 0xffff
		je cluster_read_fin


		xchg bx, bx

		and ebx, 0x0000ffff
		mov eax, ebx
		mov ebx, 2
		mul ebx
		mov esi, 0x100000
		add esi, eax
		jmp cluster_read_loop

	cluster_read_error:
		pop bx
		popad
		mov esi, ferrmsg
		call printk
		ret

	cluster_read_fin:
		popad
		ret


write_file: ; eax=cluster num
	pushad
	mov esi, 0x100000
	mov ebx, 2
	mul ebx
	add esi, eax
	mov edi, 0x140000
	cluster_write_loop:
		mov bx, word [esi]
		push bx
		cmp bx, 0xfff0
		je cluster_write_error

		; read sector
		mov cl, 1
		mov ebx, eax
		add ebx, 1536
		mov esi, 0x140000
		call ata_write

		pop bx

		cmp bx, 0xffff
		je cluster_write_fin


		xchg bx, bx

		and ebx, 0x0000ffff
		mov eax, ebx
		mov ebx, 2
		mul ebx
		mov esi, 0x100000
		add esi, eax
		jmp cluster_write_loop

	cluster_write_error:
		pop bx
		popad
		mov esi, ferrmsg
		call printk
		ret

	cluster_write_fin:
		popad
		ret

ferrmsg db 'BAD CLUSTER', 0x0d, 0
