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
SELECTOR_VIDEO	equ	0x0003<<3

protect_mode:
	lgdt [lgdt_value]
	in al, 0x92
	or al,0000_0010b
	out 0x92, al
	cli
	mov eax, cr0
	or eax, 1
	mov cr0, eax
	;jmp $
	jmp DWORD SELECTOR_CODE:main
	
[bits 32]

main:
	mov ax,SELECTOR_DATA
	mov ds,ax
	mov es,ax
	mov ss,ax
	mov gs,ax
	mov byte [gs:0xa0],'3'
	mov byte [gs:0xa2],'2'
	mov byte [gs:0xa4],'m'
	mov byte [gs:0xa6],'o'
	mov byte [gs:0xa8],'d'
	jmp $