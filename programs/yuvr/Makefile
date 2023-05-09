all:
	nasm -f elf ./yuvr.s -o ./build/yuvr.s.o
	i686-elf-gcc -g -T ./linker.ld -o ./yuvr.o -ffreestanding -O0 -nostdlib -fpic -g ./build/yuvr.s.o

clean:
	rm -rf ./build/yuvr.s.o ./yuvr.o