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
SELECTOR_DATA	equ	0x0002<<3

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

main:
	mov ax, SELECTOR_DATA
	mov ds, ax
	mov byte [ds:0], 'A'
	mov eax, 10
	mov ebx, 10
	call mov_cursor
	call cls
	hlt

	
	
%include "src/32bitutil.inc"
msg: db 'this is a hello world in 32-bit protect mode uses printf func', 0