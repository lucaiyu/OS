format_filename: ; esi=char *decoded_file_name  return esi=char *formatted_file_name
	pushad
	check_filename:
		mov ebx, 0
		find_dot:
			mov al, byte [esi+ebx]
			cmp al, '.'
			je dot_found
			inc ebx
			cmp ebx, 11
			jg format_fin
			jmp find_dot
		dot_found:
			mov ecx, ebx
			mov edx, dword [esi+ebx+1]
			mov eax, 8
			sub eax, ecx
			mov ecx, eax
			fill_0:
				mov byte [esi+ebx], 0
				inc ebx
				loop fill_0
			mov dword [esi+8], edx
			mov byte [esi+11], 0
			jmp format_fin
		format_fin:
			popad
			ret


decode_filename: ; esi=char *formatted_file_name  return esi=char *decoded_file_name
	pushad
	mov ebx, 0
	split_filename:
		mov al, byte [esi+ebx]
		cmp al, 0
		je split_found
		cmp al, 0x20
		je split_found
		inc ebx
		cmp ebx, 11
		jg decode_fin
		jmp split_filename
	split_found:
		xchg bx, bx
		mov ecx, ebx
		mov edx, dword [esi+8]
		cmp edx, 0
		je decode_fin
		mov byte [esi+ebx], '.'
		mov dword [esi+ebx+1], edx
		jmp decode_fin
	decode_fin:
		popad
		ret