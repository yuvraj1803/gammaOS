FILES = ./build/kernel.s.o ./build/kernel.o ./build/idt/idt.s.o ./build/idt/idt.o ./build/mm/memory.o ./build/io/io.s.o
INCLUDES = -I./kernel
FLAGS = -g -ffreestanding -falign-jumps -falign-functions -falign-labels -falign-loops -fstrength-reduce -fomit-frame-pointer -finline-functions -Wno-unused-function -fno-builtin -Werror -Wno-unused-label -Wno-cpp -Wno-unused-parameter -nostdlib -nostartfiles -nodefaultlibs -Wall -O0 -Iinc


all: ./bin/boot.bin ./bin/kernel.bin
	rm -rf ./bin/os.bin
	dd if=./bin/boot.bin >> ./bin/os.bin
	dd if=./bin/kernel.bin >> ./bin/os.bin
	dd if=/dev/zero bs=512 count=100 >> ./bin/os.bin

./bin/kernel.bin: $(FILES)
	i686-elf-ld -g -relocatable $(FILES) -o ./build/kernelfull.o
	i686-elf-gcc -T ./kernel/linker.ld -o ./bin/kernel.bin -ffreestanding -O0 -nostdlib ./build/kernelfull.o

./bin/boot.bin: ./boot/boot.s
	nasm -f bin ./boot/boot.s -o ./bin/boot.bin

./build/kernel.s.o: ./kernel/kernel.s
	nasm -f elf -g ./kernel/kernel.s -o ./build/kernel.s.o

./build/kernel.o: ./kernel/kernel.c
	i686-elf-gcc $(INCLUDES) $(FLAGS) -std=gnu99 -c ./kernel/kernel.c -o ./build/kernel.o 

./build/idt/idt.s.o: ./kernel/idt/idt.s
	nasm -f elf -g ./kernel/idt/idt.s -o ./build/idt/idt.s.o
	
./build/idt/idt.o: ./kernel/idt/idt.c
	i686-elf-gcc $(INCLUDES) -I./kernel/idt $(FLAGS) -std=gnu99 -c ./kernel/idt/idt.c -o ./build/idt/idt.o 

./build/mm/memory.o: ./mm/memory.c
	i686-elf-gcc $(INCLUDES) -I./mm $(FLAGS) -std=gnu99 -c ./mm/memory.c -o ./build/mm/memory.o 

./build/io/io.s.o: ./io/io.s
	nasm -f elf -g ./io/io.s -o ./build/io/io.s.o



clean:
	rm -rf ./bin/boot.bin	
	rm -rf ./bin/os.bin
	rm -rf ./bin/kernel.bin
	rm -rf $(FILES)
	rm -rf ./build/kernelfull.o
	rm -rf ./kernel/kernel.o