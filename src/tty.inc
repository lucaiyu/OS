ttyhead: db 'tty$', 0
badcommand: db 'command not found', 0

tty:
	call println
	mov si, ttyhead
	call printf
	ret