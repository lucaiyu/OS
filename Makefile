.PHONY: run
run: clean kernel.img
	-qemu-system-i386 -fda kernel.img


kernel.img: build/ build/bootsect.bin build/setup.bin build/head.bin
	-cat build/bootsect.bin > kernel.img
	-cat build/setup.bin >> kernel.img
	-cat build/head.bin >> kernel.img


build/bootsect.bin: src/boot/bootsect.asm
	nasm -fbin src/boot/bootsect.asm -o build/bootsect.bin

build/setup.bin: src/boot/setup.asm
	nasm -fbin src/boot/setup.asm -o build/setup.bin

build/head.bin: src/boot/head.asm
	nasm -fbin src/boot/head.asm -o build/head.bin

build/:
	mkdir build


.PHONY: clean
clean:
	-rm -fr build/ kernel.img


.PHONY: dbg
dbg: clean kernel.img
	-bochs -f test.cfg
