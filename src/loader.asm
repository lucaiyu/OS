[ORG 0x7c00]
[bits 16]


mov ah, 0x00
mov al, 0x03
int 0x10

mov si, msg
call print
call printnl

; load kernel
mov dh, 0x08 ;sector num
mov cl, 0x02 ;sector offset
mov bx, 0x8000; buffer ptr
call loadkernel


jmp 0x8000


%include "src/io.inc"
msg: db 'loading system...', 0

loadkernel:
	pusha
	push dx
	
	mov ah, 0x02
	mov al, dh
	mov cl, cl
	mov ch, 0x00
	mov dh, 0x00
	int 0x13
	jc kerror
	pop dx
	cmp al, dh
	jne kerror
	popa
	ret
kerror:
	mov si, ke
	call print
	jmp $
	ret

ke db 'FAILED TO LOAD KERNEL', 0

times 510-($-$$) db 0
db 0x55,0xaa