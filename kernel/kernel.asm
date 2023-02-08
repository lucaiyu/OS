[bits 32]

main:
	mov esp,0x1e25c
	mov byte [0xb8000], 'M'
	jmp $