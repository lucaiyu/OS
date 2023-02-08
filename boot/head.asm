%include "boot/config.inc"

SETUPSEG equ DEF_SETUPSEG
SYSSEG equ DEF_SYSSEG

_pg_dir equ 0x0000

pg0 equ  0x1000
pg1 equ  0x2000
pg2 equ  0x3000
pg3 equ  0x4000

_tmp_floppy_area equ 0x5000
len_floppy_area equ 0x400



[bits 32]
jmp entry

entry:
	mov eax, 2*8 ; reset segment selector
	mov ds, eax
	mov es, eax
	mov fs, eax
	mov gs, eax
	mov ss, eax

	mov esp,0x1e25c

	mov esi, pmsg
	mov cl, 0x0f
	mov edi, 0xb8000+6*160
	call printt


	call setup_idt
	call setup_gdt

	jmp 1*8:newgdt
		nop
		nop

newgdt:
	mov esi, resetmsg
	mov cl, 0x0f
	mov edi, 0xb8000+7*160
	call printt

	sti
	int 0x80
	cli

	call opena20

	mov esi, a20
	mov cl, 0x0f
	mov edi, 0xb8000+8*160

	call setup_paging

	push panic
	jmp 1*8:main
	jmp $



setup_paging:
	mov dword [_pg_dir], pg0+7
	mov dword [_pg_dir+4], pg1+7
	mov dword [_pg_dir+8], pg2+7
	mov dword [_pg_dir+12], pg3+7

	mov edi, pg3+4092
	mov eax, 0xfff007

	std
	go_on:
		stosd
		sub eax, 0x1000
		jge go_on

	xor eax, eax
	mov cr3, eax
	mov eax, cr0
	or eax, 0x80000000
	mov cr0, eax
	ret

panic:
	mov esi, panicmsg
	mov cl, 0x04
	mov edi, 0xb8000+12*160
	call printt
	jmp $


keyboard_test:
	mov al, 11111101b
	out 021h, al
	dw 0x00eb,0x00eb
	mov al, 11111111b
	out 0A1h, al
	dw	0x00eb,0x00eb
	ret


printt:
        mov  bl ,[esi]
        cmp  bl, 0
        je   printfin
        mov  byte [edi],bl
        inc  edi
        mov  byte [edi],cl
        inc  esi
        inc  edi
        jmp  printt
printfin:
        ret


opena20:
	xor eax, eax
    inc eax 
   	mov [0x000000],eax   
    cmp eax,[0x100000]
    je opena20
    ret



setup_idt:
	lea edx, [generic_int]
	mov eax,0x00080000
	mov ax, dx
	mov dx,0x8E00
	lea edi,[_idt]
	mov ecx, 256
	idt_loop:
		mov [edi],eax
        mov [edi+4],edx
        add  edi,8
        loop idt_loop
              
        lidt [idt_descr]
        ret   
	


generic_int:
	cli
	pushad

	push ds
	push es
	push fs
	push gs
	push ss

	mov eax, 2*8
	mov ds, eax
	mov es, eax
	mov fs, eax
	mov gs, eax
	mov ss, eax

	mov esi, intmsg
	mov cl, 0x0f
	mov edi, 0xb8000+8*160
	call printt

	pop ss
	pop gs
	pop fs
	pop es
	pop ds
	popad
	iret

align 2
dw 0


idt_descr:
    dw 256*8-1
    dd _idt                               
    ret

setup_gdt:
    lgdt [gdt_descr]
    ret

align 2
dw 0

gdt_descr:
        dw 256*8-1  
        dd _gdt 



pmsg db 'already in protect mode', 0
intmsg db 'soft int ok', 0
resetmsg db 'reseting GDT&IDT', 0
a20 db 'A20 line is open', 0
mainmsg db 'kernel main loaded', 0
panicmsg db 'KERNEL PANIC: kernel main die!', 0


_idt: times 256 dq 0
_gdt: dq 0x0000000000000000 ; NULL SEGMENT r-w-x
      dq 0x00c09a0000000fff ; CODE SEGMENT R-w-X
      dq 0x00c0920000000fff ; DATA SEGmENT R-W-x
      dq 0x0000000000000000 ; RESERVED SEGMENT r-w-x
      times 252 dq 0

%include "kernel/kernel.asm"
