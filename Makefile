.PHONY: run
run: clean kernel.img
	-bochs -f bochsrc.bxrc


kernel.img: build/ build/loader.bin build/kernel.bin
	-cp build/loader.bin kernel.img
	-cat build/kernel.bin >> kernel.img


build/loader.bin: loader.asm
	nasm -fbin loader.asm -o build/loader.bin	
	
build/kernel.bin: kernel.asm
	nasm -fbin kernel.asm -o build/kernel.bin

build/:
	mkdir build


.PHONY: clean
clean:
	-rm -fr build/ kernel.img
