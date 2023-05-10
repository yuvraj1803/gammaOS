OBJ = ./build/_start.s.o ./build/stdio.o
INC = ./include

all: $(OBJ)
	i686-elf-ld -m elf_i386 -relocatable $(OBJ) -o ./lib.o

./build/_start.s.o: ./_start/_start.s
	nasm -f elf ./_start/_start.s -o ./build/_start.s.o

./build/stdio.o: ./stdio/stdio.s
	nasm -f elf ./stdio/stdio.s  -o ./build/stdio.o

clean:
	rm -rf $(OBJ)
	rm -rf ./lib.o