%include "boot/config.inc"

SETUPSEG equ DEF_SETUPSEG
SYSSEG equ DEF_SYSSEG

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

	push panic
	jmp main


%include "kernel/kernel.asm"


panic:
	mov esi, panicmsg
	mov cl, 0x04
	mov edi, 0xb8000+12*160
	call printt
	jmp $


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
	lea edi, [_idt]
	add edi, 8*0x80
	mov [edi], eax
	mov [edi+4], edx
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
_gdt: dw 0, 0, 0, 0 ; NULL SEGMENT r-w-x
      ; KERNEL CODE SEGMENT R-w-X
      dw 0x3fff ; limit low
      dw 0 ; base_low
      dw 0x9a00 ; access, base_middle
      dw 0x00c0 ; base_high, flag, limit_high

      ; KERNEL DATA SEGMENT R-W-x
      dw 0x3fff ; limit low
      dw 0 ; base_low
      dw 0x9200 ; access, base_middle
      dw 0x00c0 ; base_high, flag, limit_high

      ; USER CODE SEGMENT R-w-X
      dw 0xffff ; limit low
      dw 0x4000 ; base_low
      dw 0xfa00 ; access, base_middle
      dw 0x00c0 ; base_high, flag, limit_high

      ; USER DATA SEGMENT R-W-x
      dw 0xffff ; limit low
      dw 0x4000 ; base_low
      dw 0xf200 ; access, base_middle
      dw 0x00c0 ; base_high, flag, limit_high
      times 252 dq 0
