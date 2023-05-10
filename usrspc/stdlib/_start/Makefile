FILES = ./build/_start.s.o

all: $(FILES)
	i686-elf-ld -m elf_i386 -relocatable $(FILES) -o ./_start.o

./build/_start.s.o: ./_start.s
	nasm -f elf ./_start.s -o ./build/_start.s.o

clean:
	rm -rf ./build/_start.s.o ./_start.o