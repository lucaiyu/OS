.PHONY: run
run: clean kernel.img
	-bochs -f bochsrc.bxrc


kernel.img: build/ build/loader.bin build/kernel.bin
	-cp build/loader.bin kernel.img
	-cat build/kernel.bin >> kernel.img


build/loader.bin: src/loader.asm
	nasm -fbin src/loader.asm -o build/loader.bin	
	
build/kernel.bin: src/kernel.asm
	nasm -fbin src/kernel.asm -o build/kernel.bin

build/:
	mkdir build


.PHONY: clean
clean:
	-rm -fr build/ kernel.img
