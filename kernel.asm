[ORG 0x9000]

mov si, msg
call print
call printnl


jmp $



%include "biosutil.inc"
msg: db 'kernel on sector 2 loaded', 0
times 512-($-$$) db 0