OBJ = ./build/_start.s.o

all: $(OBJ)
	i686-elf-ld -m elf_i386 -relocatable $(OBJ) -o ./_start.o

./build/_start.s.o: ./_start.s
	nasm -f elf ./_start.s -o ./build/_start.s.o

clean:
	rm -rf ./build/_start.s.o ./_start.o