FAT_ADDR equ 0x100000
ROOT_ADDR equ 0x120000
DATA_ADDR equ 0x140000

ferrmsg db 'BAD CLUSTER', 0x0d, 0
createerrmsg db 'FAILED TO CREATE FILE: ROOT TABLE IS FULL', 0x0d, 0
fopenerrmsg db 'Warning! file not found!', 0x0d, 0



; file struct (length 32 bytes)
; 0x00-0x07: name
; 0x08-0x0a: extension name
; 0x0b: attribute 00000000b-file 00010000b-dir (ORG FAT16 REQUEST)
; 0x0c-0x0d: addational info -- permission r-w-x -> 9bits
;							 -- flag 0000000b-empty 0000001b-file 0000010b-dir -> 7bits
; 0xoe-0x15: reserved
; 0x16-0x17: mod time
; 0x18-0x19: mod date
; 0x1A-0x1B: file head cluster num
; 0x1C-0x1F: file size


fopen: ; esi=char *name  like  foo.bar  return eax=handle
	pushad
	mov edi, ROOT_ADDR
	call format_filename
	find_file_loop:
	xor ebx, ebx
		compare_name_loop:
			mov al, byte [esi+ebx]
			cmp al, byte [edi+ebx]
			jne comapre_failed
			inc ebx
			cmp ebx, 11
			jge file_name_found
			jmp compare_name_loop
		comapre_failed:
			add edi, 32
			cmp edi, DATA_ADDR
			jge fopen_failed
			jmp find_file_loop
	file_name_found:
		xor eax, eax
		mov ax, word [edi+0x1a]
		mov dword [0x90030], eax
		popad
		mov eax, dword [0x90030]
		ret
	fopen_failed:
		mov esi, fopenerrmsg
		call printk
		popad
		ret

fclose: ; eax=handle
	call write_file
	call fs_sync
	ret

create_file: ; esi=char *name  like  foo00000bar
	pushad
	mov edi, ROOT_ADDR
	get_empty_struct:
		cmp byte [edi+0x0d], 0
		je write_file_info
		add edi, 32
		cmp edi, DATA_ADDR
		jge create_error
		jmp get_empty_struct
	write_file_info:
		push edi
		xor ecx, ecx
		write_file_name:
			mov al, [esi]
			mov [edi], al
			inc edi
			inc esi
			inc ecx
			cmp ecx, 11
			jge write_file_attribute
			jmp write_file_name
		write_file_attribute:
			pop edi
			mov byte [edi+0x0b], 0
			mov byte [edi+0x0c], 11111111b
			mov byte [edi+0x0d], 10000001b
			call get_empty_cluster
			mov word [edi+0x1a], ax
			; add cluster sig
			mov edi, 0x100000
			and eax, 0x0000ffff
			shl eax, 1
			add edi, eax
			mov word [edi], 0xffff
			popad
			call fs_sync
			ret
	create_error:
		mov esi, createerrmsg
		call printk
		popad
		ret


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

fs_sync:
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


read_file: ; eax=cluster num  edi=buffer addr
	pushad
	mov esi, FAT_ADDR
	mov ebx, 2
	mul ebx
	add esi, eax
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


write_file: ; eax=cluster num edi=buffer addr
	pushad
	mov esi, 0x100000
	mov ebx, 2
	mul ebx
	add esi, eax
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


get_empty_cluster: ; return ax=cluster num
	push edi
	mov edi, 0x100000
	get_empty_cluster_loop:
		cmp word [edi], 0
		je get_empty_cluster_fin
		add edi, 2
		cmp edi, 0x120000
		jge create_error
		jmp get_empty_cluster_loop
	get_empty_cluster_fin:
		sub edi, 0x100000
		shr edi, 1
		mov ax, di
		pop edi
		ret

