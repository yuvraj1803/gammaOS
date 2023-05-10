OBJ= ./build/init.o
INC= -I../lib/include
FLAGS = -g -ffreestanding -falign-jumps -falign-functions -falign-labels -falign-loops -fstrength-reduce -fomit-frame-pointer -finline-functions -Wno-unused-function -fno-builtin -Werror -Wno-unused-label -Wno-cpp -Wno-unused-parameter -Wno-unused-variable -Wno-unused-value  -nostdlib -nostartfiles -nodefaultlibs -Wall -O0 -Iinc

all: $(OBJ)
	i686-elf-gcc -g -T ../linker.ld -o ./init.o -ffreestanding -O0 -nostdlib -fpic -g $(OBJ) ../lib/lib.o

# ./build/init.o is not linked.
# ./init.o is linked.
./build/init.o: ./init.c
	i686-elf-gcc $(INC) -I./ $(FLAGS) -std=gnu99 -c ./init.c -o ./build/init.o 

clean:
	rm -rf ./build/init.o ./init.o