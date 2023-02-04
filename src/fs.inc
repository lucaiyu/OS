; a simple filesystem
; sector 1 mbr
; sector 2-9 kernel code
; sector 10-17 fat
; sector 18-2880 file data
; 
; max files: 1024
; max file size: 10MB(20sectors)
; 
; fat struct
; size: 1xdword -> 4bytes
; struct fat{
; 	word id
;	word begin_sector
; };


listfile:
	pusha
	mov di, 0x9006
	mov si, spaces
	filels:
		cmp word [di-6], 0x0000
		je lsend
		call printfilename
		call print
		add di, 16
		jmp filels
	lsend:
		popa
		ret

printfilename: ;di=char *str
	pusha
	xor bx, bx
	mov ah, 0x0e
	first:
		mov al, byte [di+bx]
		inc bx
		cmp al, ' '
		je firstfin
		cmp al, 0
		je firstfin
		int 0x10
		cmp bx, 0x0a
		jge firstfin
		jmp first

	firstfin:
		popa
		ret



createfile: ;si=char *filename
	pusha
	mov bx, 0x9000
	mov cx, 0
	findend:
		cmp word [bx], 0x0000
		je wintofs
		add bx, 4
		inc cx
		cmp bx, 0x9000+4096 ;256 files max
		jge createerr
		jmp findend
	createerr:
		mov si, toucherrmsg
		call print
		call printnl
		popa
		ret
	wintofs:
		inc cx
		mov dword [bx], 0xffffffff
		mov ax, 20
		mul cx
		mov dword [bx+4], 0xffffffff
		mov dh, 0x01
		mov cl, 0x0a
		mov bx, 0x9000
		call wdisk
		popa
		ret



spaces db '  ', 0
toucherrmsg db 'unable to touch file: filesystem table is full', 0