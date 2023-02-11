FAT_ADDR equ 0x100000
ROOT_ADDR equ 0x120000
DATA_ADDR equ 0x140000



; file struct (length 32 bytes)
; 0x00-0x07: name
; 0x08-0x0a: extension name
; 0x0b-0x0d: permission (r-w-x)
; 0x0e: attribute 00000001-normal 00000010-read only 00000100-dir 00000000-unused
; 0x0f-0x10: begin cluster num
; 0x11-0x12: cluster count
; 0x13-0x1f: date data(optional) default 0


create_file: ;esi=char *name  like  foo.bar
	pushad
	mov edx, esi
	; check filename
	mov ecx, 0
	xchg bx, bx
	filename_check_loop:
		mov al, [esi]
		cmp al, '.'
		je extension_name_check_loop
		; cmp al, ' '
		; jbe illegal_filename
		; cmp al, '/'
		; jbe illegal_filename
		; cmp al, '~'
		; jg illegal_filename
		inc esi
		inc ecx
		; cmp ecx, 9
		; jg illegal_filename
		jmp filename_check_loop

	mov ecx, 0

	xchg bx, bx
	extension_name_check_loop:
		mov al, [esi]
		cmp al, 0
		je find_empty_struct_loop
		; cmp al, ' '
		; jbe illegal_filename
		; cmp al, '/'
		; jbe illegal_filename
		; cmp al, '~'
		; jg illegal_filename
		inc esi
		inc ecx
		; cmp ecx, 5
		; jg illegal_filename
		jmp extension_name_check_loop

	xchg bx, bx

	mov edi, 0x12000c
	find_empty_struct_loop:
		cmp byte [edi], 0
		je start_create_file
		add edi, 32
		cmp edi, 0x140000
		jge fsfullerr
		jmp find_empty_struct_loop

	start_create_file:
		sub edi, 0x0c
		push edi
		mov esi, edx
		write_name_loop:
			cmp byte [esi], '.'
			je write_extension
			mov al, byte [esi]
			mov byte [edi], al
			inc esi
			inc edi
			jmp write_name_loop
		write_extension:
			pop edi
			push edi
			add edi, 0x08
			write_extension_loop:
				cmp byte [esi], 0
				je create_fin
				mov al, byte [esi]
				mov byte [edi], al
				inc esi
				inc edi
				jmp write_extension_loop
	create_fin:
		; write permission
		pop edi
		mov byte [edi+0x0b], 7
		mov byte [edi+0x0c], 7
		mov byte [edi+0x0d], 7
		mov byte [edi+0x0e], 00000001b

		popad
		call fs_write
		ret

illfn db 'Illegal file name!', 0x0d, 0
fsferr db 'Filesystem table is full!', 0x0d, 0

	illegal_filename:
		popad
		mov esi, illfn
		call printk
		ret
	fsfullerr:
		popad
		mov esi, fsferr
		call printk
		ret


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



get_unused_cluster: ; return eax=cluster num



ferrmsg db 'BAD CLUSTER', 0x0d, 0
