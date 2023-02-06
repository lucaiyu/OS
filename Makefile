.PHONY: run
run: clean kernel.img data.img
	-qemu-system-x86_64 -fda kernel.img -fdb data.img


kernel.img: build/ build/loader.bin build/kernel.bin
	-cp build/loader.bin kernel.img
	-cat build/kernel.bin >> kernel.img

data.img: build/ build/data.bin build/fat12.bin build/datambr.bin
	-cp build/datambr.bin data.img
	-cat build/fat12.bin >> data.img
	-cat build/data.bin >> data.img


build/loader.bin: src/loader.asm
	nasm -fbin src/loader.asm -o build/loader.bin	
	
build/kernel.bin: src/kernel.asm
	nasm -fbin src/kernel.asm -o build/kernel.bin

build/fat12.bin: src/fat12.bin.asm
	nasm -fbin src/fat12.bin.asm -o build/fat12.bin

build/data.bin: src/data.bin.asm
	nasm -fbin src/data.bin.asm -o build/data.bin

build/datambr.bin: src/datambr.asm
	nasm -fbin src/datambr.asm -o build/datambr.bin

build/:
	mkdir build


.PHONY: clean
clean:
	-rm -fr build/ kernel.img data.img


.PHONY: dbg
dbg: clean kernel.img data.img
	-bochs -f test.cfg
