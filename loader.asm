[ORG 0x7c00]



mov ah, 0x00
mov al, 0x03
int 0x10

mov si, msg
call print

call printnl

mov dh, 0x01 ;sector num
mov cl, 0x02 ;sector offset
mov bx, 0x9000; buffer ptr
call disk

jmp 0x9000


%include "biosutil.inc"
msg: db 'loading kernel on sector 2', 0

times 510-($-$$) db 0
db 0x55,0xaa