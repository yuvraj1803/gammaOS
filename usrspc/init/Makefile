OBJ= ./build/init.o

all: $(OBJ)
	i686-elf-gcc -g -T ../linker.ld -o ./init.o -ffreestanding -O0 -nostdlib -fpic -g $(OBJ) ../stdlib/_start/_start.o

./build/init.o: ./init.c
	i686-elf-gcc -I./ -std=gnu99 -c ./init.c -o ./build/init.o 

clean:
	rm -rf ./build/init.o ./init.o