cls:
	mov eax, 0x00000000
clear_screen:
	mov byte [gs:eax], 0x00
	add eax, 2
	cmp eax, 0x00000f9f
	jge fin
	jmp clear_screen
fin:
	mov bx, 0
    mov dx, 0x3d4
    mov al, 0x0e
    out dx, al
    mov dx, 0x3d5
    mov al, bh
    out dx, al
    mov dx, 0x3d4
    mov al, 0x0f
    out dx, al
    mov dx, 0x3d5
    mov al, bl
    out dx, al
	mov esp, 0x900
	ret