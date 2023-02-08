[bits 32]

main:
	mov eax, 2*8 ; reset segment selector
	mov ds, eax
	mov es, eax
	mov fs, eax
	mov gs, eax
	mov ss, eax

	mov esp,0x1e25c
	mov byte [0xb8000], 'M'
	jmp $