[ORG 0x900]

jmp protect_mode

gdt:
	dd 0x00000000
	dd 0x00000000
	dd 0x0000ffff
	dd 0x00cf9800
	dd 0x80000007
	dd 0x00c0920b
	
lgdt_value:
	dw $-gdt-1
	dd gdt
	
SELECTOR_CODE	equ	0x0001<<3
SELECTOR_VIDEO	equ	0x0002<<3

protect_mode:
	cli
	lgdt [lgdt_value]
	in al, 0x92
	or al,0000_0010b
	out 0x92, al
	mov eax, cr0
	or eax, 1
	mov cr0, eax
	jmp DWORD SELECTOR_CODE:main
	
[bits 32]
; segments:
	;code segment:cs(auto do not mod)
	;data segment:ds(suto 0x0000)
	;video segment:gs(offset 0x8B000)
main:
	
	mov ax, SELECTOR_VIDEO
	mov gs, ax
	
	call println
	mov bx, 0xa0
	mov si, msg
	call printf
	call println

	intt:
		call getchar
		jmp intt
	hlt

	
	
%include "src/32bitutil.inc"
%include "src/tty.inc"
msg: db 'gdt loaded', 0