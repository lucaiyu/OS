tty:
	mov si, ttysym
	mov bx, 0
	call print
	scan:
		; keyborad int scan(sync)
		mov ah, 0x00
		int 0x16
		cmp al, 0x0d
		je analyze
		; if buffer is full -> jmp
		cmp bx, 255
		jge scan
		cmp al, 0x20
		jb scan
		; if keyboard enters control char -> jmp
		mov ah, 0x0e
		int 0x10
		; show char
		mov byte [combuffer+bx], al
		inc bx
		; push char into stack
		jmp scan



	analyze:
		call printnl
		cmp byte [combuffer], 0x00
		je tty
		mov si, combuffer
		call print
		mov bx, 0
		call clearbuf
		call printnl
		jmp tty

	clearbuf:
		mov byte [combuffer+bx], 0x00
		inc bx
		cmp bx, 255
		jge clearfin
		jmp clearbuf
		clearfin:
			ret















ttysym: db 'TTY$', 0
comerr: db 'command not found', 0
debug: db 'buffer cleared'
combuffer: times 255 db 0