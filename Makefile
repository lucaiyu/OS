.PHONY: run
run: clean kernel.img
	-bochs -f test.cfg


kernel.img: build/ build/loader.bin build/kernel.bin build/fs.bin
	-cp build/loader.bin kernel.img
	-cat build/kernel.bin >> kernel.img
	-cat build/fs.bin >> kernel.img


build/loader.bin: src/loader.asm
	nasm -fbin src/loader.asm -o build/loader.bin	
	
build/kernel.bin: src/kernel.asm
	nasm -fbin src/kernel.asm -o build/kernel.bin

build/fs.bin: src/fs.bin.asm
	nasm -fbin src/fs.bin.asm -o build/fs.bin

build/:
	mkdir build


.PHONY: clean
clean:
	-rm -fr build/ kernel.img


.PHONY: start
start:
	-bochs -f test.cfg
