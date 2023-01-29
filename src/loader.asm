[ORG 0x7c00]
[bits 16]


mov ah, 0x00
mov al, 0x03
int 0x10

mov si, msg
call print
call printnl

mov dh, 0x01 ;sector num
mov cl, 0x02 ;sector offset
mov bx, 0x8000; buffer ptr
call disk

jmp 0x8000


%include "src/io.inc"
msg: db 'loading system...', 0

times 510-($-$$) db 0
db 0x55,0xaa