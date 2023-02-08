;%define debug
%ifdef debug
isdebug equ 1
%else
isdebug equ 0
%endif

DEF_INITSEG	equ	0x9000
DEF_SYSSEG	equ	0x1000
DEF_SETUPSEG equ 0x9020