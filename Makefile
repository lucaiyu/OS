.PHONY: run
run: kernel.img
	-bochs -f test.cfg


kernel.img: build/ build/loader.bin
	-cp build/loader.bin kernel.img


build/loader.bin: loader.asm
	nasm -fbin loader.asm -o build/loader.bin	

build/:
	mkdir build


.PHONY: clean
clean:
	-rm -fr build/ kernel.img
