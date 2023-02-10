; eax=LBA, cx=sector_num, ebx=buffer_addr
ata_read:
	pushad
	mov esi, eax
	mov di, cx

	mov dx, 0x1f2
	mov al, cl
	out dx, al
	mov eax, esi

	mov cl, 8
	mov dx, 0x1f3
	out dx, al

	mov dx, 0x1f4
	shr eax, cl
	out dx, al

	mov dx, 0x1f5
	shr eax, cl
	out dx, al

	mov dx, 0x1f6
	shr eax, cl
	and al, 0x0f
	or al, 0xe0
	out dx, al

	mov dx, 0x1f7
	mov al, 0x20
	out dx, al

	.not_ready:
		nop
		in al, dx
		and al, 0x88
		and al, 0x08
		jnz .not_ready

	mov ax, di
	mov dx, 256
	mul dx
	mov cx, ax
	xchg bx, bx

	mov dx, 0x1f0
	xchg bx, bx
	.go_on_read:
		in ax, dx
		mov [ebx], ax
		add ebx, 2
		loop .go_on_read
	xchg bx, bx
	popad
	ret


ata_chs_read:
				pushad

 
                mov edx,1f6h            ;port to send drive & head numbers
                mov al,bh               ;head index in BH
                and al,00001111b        ;head is only 4 bits long
                or  al,10100000b        ;default 1010b in high nibble
                out dx,al
 
                mov edx,1f2h            ;Sector count port
                mov al,ch               ;Read CH sectors
                out dx,al
 
                mov edx,1f3h            ;Sector number port
                mov al,bl               ;BL is sector index
                out dx,al
 
                mov edx,1f4h            ;Cylinder low port
                mov eax,ebx             ;byte 2 in ebx, just above BH
                mov cl,16
                shr eax,cl              ;shift down to AL
                out dx,al
 
                mov edx,1f5h            ;Cylinder high port
                mov eax,ebx             ;byte 3 in ebx, just above byte 2
                mov cl,24
                shr eax,cl              ;shift down to AL
                out dx,al
 
                mov edx,1f7h            ;Command port
                mov al,20h              ;Read with retry.
                out dx,al
 
.still_going:   in al,dx
                test al,8               ;the sector buffer requires servicing.
                jz .still_going         ;until the sector buffer is ready.
 
                mov eax,512/2           ;to read 256 words = 1 sector
                xor bx,bx
                mov bl,ch               ;read CH sectors
                mul bx
                mov ecx,eax             ;RCX is counter for INSW
                mov edx,1f0h            ;Data port, in and out
                rep insw                ;in to [RDI]

                popad
                ret