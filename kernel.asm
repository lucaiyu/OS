; mov si, msg
; call start
; jmp $


; print:
	; pusha
	; jmp start
; start:
	; mov al, [si]
	; cmp al, 0
	; je fin
	; mov ah, 0x0e
	; int 0x10
	; inc si
	; jmp start
; fin:
	; popa
	; ret



; msg: db 'kernel on sector 2 loaded', 0
db 'ABC', 0
times 512-($-$$) db 0