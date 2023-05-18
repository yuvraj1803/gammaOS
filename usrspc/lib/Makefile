OBJ = ./build/_start.s.o ./build/stdio.s.o ./build/stdlib.s.o ./build/stdio.o ./build/stdlib.o ./build/unistd.s.o \
		./build/string.o ./build/_exit.s.o
INC = -I./include
FLAGS = -g -ffreestanding -falign-jumps -falign-functions -falign-labels -falign-loops -fstrength-reduce -fomit-frame-pointer -finline-functions -Wno-unused-function -fno-builtin -Werror -Wno-unused-label -Wno-cpp -Wno-unused-parameter -Wno-unused-variable -Wno-unused-value  -nostdlib -nostartfiles -nodefaultlibs -Wall -O0 -Iinc

all: $(OBJ)
	i686-elf-ld -m elf_i386 -relocatable $(OBJ) -o ./lib.o

./build/_start.s.o: ./_start/_start.s
	nasm -f elf ./_start/_start.s -o ./build/_start.s.o

./build/_exit.s.o: ./_exit/_exit.s;
	nasm -f elf ./_exit/_exit.s -o ./build/_exit.s.o

./build/stdio.s.o: ./stdio/stdio.s
	nasm -f elf ./stdio/stdio.s  -o ./build/stdio.s.o

./build/stdio.o: ./stdio/stdio.c
	i686-elf-gcc $(INC) $(FLAGS) -std=gnu99 -c ./stdio/stdio.c -o ./build/stdio.o 

./build/stdlib.o: ./stdlib/stdlib.c
	i686-elf-gcc $(INC) $(FLAGS) -std=gnu99 -c ./stdlib/stdlib.c -o ./build/stdlib.o 

./build/stdlib.s.o:
	nasm -f elf ./stdlib/stdlib.s -o ./build/stdlib.s.o 

./build/unistd.s.o:
	nasm -f elf ./unistd/unistd.s -o ./build/unistd.s.o

./build/string.o:
	i686-elf-gcc $(INC) $(FLAGS) -std=gnu99 -c ./string/string.c -o ./build/string.o 


clean:
	rm -rf $(OBJ)
	rm -rf ./lib.o