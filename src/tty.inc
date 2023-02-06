tty:
	mov si, ttysym
	mov bx, 0
	call print
	scan:
		; keyborad int scan(sync)
		mov ah, 0x00
		int 0x16
		cmp ah, 0x0e
		je backspace
		; if is enter -> jmp to command analyzer
		cmp ah, 0x1c
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

		clearcom:           
			mov    si,0
           	mov    cx,6
			findclearcom:
				mov ah, [comclear+si]
           		mov al, [combuffer+si]
           		cmp ah, al
           		jne haltcom
           		inc si
           		loop findclearcom
           		jmp clear


        haltcom:           
			mov    si,0
           	mov    cx,5
			findhaltcom:
				mov ah, [comhalt+si]
           		mov al, [combuffer+si]
           		cmp ah, al
           		jne comerror
           		inc si
           		loop findhaltcom
           		call shutdown
           		jmp comanalyzefin


        comerror:
        	mov si, combuffer
        	call print
        	mov si, comerr
			call print
			jmp comanalyzefin
		comanalyzefin:
			call clearbuf
			call printnl
			jmp tty

	clear:
		pusha
		mov ah, 0x00
		mov al, 0x03
		int 0x10
		popa
		call clearbuf
		jmp tty

	clearbuf:
		mov bx, 0
		bufclear:
			mov byte [combuffer+bx], 0x00
			inc bx
			cmp bx, 255
			jge clearfin
			jmp bufclear
		clearfin:
			ret


	backspace:
		pusha
		push bx
		mov ah, 0x03
		mov bx, 0x00
		int 0x10
		cmp dl, 0x06
		jbe return
		dec dl
		mov ah, 0x02
		int 0x10
		mov al, ' '
		mov ah, 0x0a
		mov bh, 0
		mov cx, 1
		int 0x10
		pop bx
		mov byte [combuffer+bx-1], 0x00
		popa
		sub bx, 1
		jmp scan
		return:
			pop bx
			popa
			jmp scan

	shutdown:
  		mov ax, 0x1000
  		mov ax, ss
  		mov sp, 0xf000
  		mov ax, 0x5307
  		mov bx, 0x0001
  		mov cx, 0x0003
  		int 0x15
 
  		ret






ttysym db 'shell$', 0
comerr db ': command not found', 0
comclear db 'clear', 0
comhalt db 'halt', 0

combuffer times 255 db 0