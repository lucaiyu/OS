FAT_ADDR equ 0xa00000
ROOT_ADDR equ 0xa20000
DATA_ADDR equ 0xa40000



; file struct (length 32 bytes)
; 0x00-0x07: name
; 0x08-0x0a: extension name
; 0x0b-0x0d: permission (r-w-x)
; 0x0e: attribute 00000001-normal 00000010-read only 00000100-dir 00000000-unused
; 0x0f-0x10: begin cluster num
; 0x11-0x12: cluster count
; 0x13-0x1f: date data(optional) default 0


fs_init:
	pushad
	mov cl, 128
	mov ebx, 1280
	mov edi, FAT_ADDR
	call ata_read

	mov cl, 128
	mov ebx, 1408
	mov edi, ROOT_ADDR
	call ata_read



	popad
	ret

fs_write:
	pushad

	mov cl, 128
	mov ebx, 1280
	mov esi, FAT_ADDR
	call ata_write

	mov cl, 128
	mov ebx, 1408
	mov esi, ROOT_ADDR
	call ata_write

	popad
	ret


read_file: ; eax=cluster num
	pushad
	mov esi, FAT_ADDR
	mov ebx, 2
	mul ebx
	add esi, eax
	mov edi, DATA_ADDR
	cluster_read_loop:
		mov bx, word [esi]
		push bx
		cmp bx, 0xfff0
		je cluster_read_error

		; read sector
		mov cl, 1
		mov ebx, eax
		add ebx, 1536
		mov edi, DATA_ADDR
		call ata_read

		pop bx

		cmp bx, 0xffff
		je cluster_read_fin

		and ebx, 0x0000ffff
		mov eax, ebx
		mov ebx, 2
		mul ebx
		mov esi, FAT_ADDR
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



get_unused_cluster: ; return eax=cluster num



ferrmsg db 'BAD CLUSTER', 0x0d, 0
