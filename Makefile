default :
	make run


boot/bootsect.bin : boot/bootsect.asm boot/config.inc Makefile
	nasm boot/bootsect.asm -o boot/bootsect.bin

boot/setup.bin : boot/setup.asm boot/config.inc Makefile
	nasm boot/setup.asm -o boot/setup.bin

boot/head.bin : boot/head.asm boot/config.inc Makefile
	nasm boot/head.asm -o boot/head.bin

boot/boot_setup.bin : boot/bootsect.bin boot/setup.bin boot/head.bin Makefile
	cat  boot/bootsect.bin boot/setup.bin boot/head.bin > boot/boot_setup.bin

clean:
	rm boot/bootsect.bin
	rm boot/setup.bin
	rm boot/head.bin
	rm boot/boot_setup.bin



buildimg : boot/boot_setup.bin Makefile
	cat  boot/boot_setup.bin > kernel.img


run : clean buildimg
	qemu-system-i386 -fda kernel.img

dbg : clean buildimg
	bochsdbg -f bochsrc.bxrc
