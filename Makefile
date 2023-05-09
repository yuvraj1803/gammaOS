FILES = ./build/kernel.s.o ./build/kernel.o ./build/idt/idt.S.o \
./build/idt/idt.o ./build/mm/memory.o ./build/io/io.s.o ./build/mm/heap/heap.o \
./build/mm/heap/kheap.o ./build/mm/paging/paging.s.o ./build/mm/paging/paging.o \
./build/drivers/disk/disk.o ./build/drivers/display/vga/vga.o ./build/fs/parser/parser.o \
./build/string/string.o ./build/gdt/gdt.o ./build/gdt/gdt.s.o \
./build/task/tss/tss.s.o ./build/task/task.o ./build/fs/fat16/fat16.o ./build/fs/vfs/vfs.o \
./build/fs/file.o ./build/task/process/process.o ./build/task/task.s.o ./build/idt/__0x80/__0x80.o \
 ./build/idt/__0x80/io/io.o ./build/drivers/keyboard/keyboard.o ./build/drivers/keyboard/ps2/ps2.o \
 ./build/g-loader/elf/elf.o ./build/g-loader/g-loader.o

 
INCLUDES = -I./kernel
FLAGS = -g -ffreestanding -falign-jumps -falign-functions -falign-labels -falign-loops -fstrength-reduce -fomit-frame-pointer -finline-functions -Wno-unused-function -fno-builtin -Werror -Wno-unused-label -Wno-cpp -Wno-unused-parameter -Wno-unused-variable -Wno-unused-value  -nostdlib -nostartfiles -nodefaultlibs -Wall -O0 -Iinc


all: ./bin/boot.bin ./bin/kernel.bin make_programs
	rm -rf ./bin/os.bin
	dd if=./bin/boot.bin >> ./bin/os.bin
	dd if=./bin/kernel.bin >> ./bin/os.bin
	dd if=/dev/zero bs=1048576 count=16 >> ./bin/os.bin

	sudo mount -t vfat ./bin/os.bin /mnt/d
	sudo cp -r ./programs /mnt/d

	sudo umount /mnt/d

./bin/kernel.bin: $(FILES)
	i686-elf-ld -g -relocatable $(FILES) -o ./build/kernelfull.o
	i686-elf-gcc -T ./kernel/linker.ld -o ./bin/kernel.bin -ffreestanding -O0 -nostdlib ./build/kernelfull.o

./bin/boot.bin: ./boot/boot.s
	nasm -f bin ./boot/boot.s -o ./bin/boot.bin

./build/kernel.s.o: ./kernel/kernel.s
	nasm -f elf -g ./kernel/kernel.s -o ./build/kernel.s.o

./build/kernel.o: ./kernel/kernel.c
	i686-elf-gcc $(INCLUDES) $(FLAGS) -std=gnu99 -c ./kernel/kernel.c -o ./build/kernel.o 

./build/idt/idt.S.o: ./kernel/idt/idt.S
	nasm -f elf -g ./kernel/idt/idt.S -o ./build/idt/idt.S.o
	
./build/idt/idt.o: ./kernel/idt/idt.c
	i686-elf-gcc $(INCLUDES) -I./kernel/idt $(FLAGS) -std=gnu99 -c ./kernel/idt/idt.c -o ./build/idt/idt.o 

./build/mm/memory.o: ./mm/memory.c
	i686-elf-gcc $(INCLUDES) -I./mm $(FLAGS) -std=gnu99 -c ./mm/memory.c -o ./build/mm/memory.o 

./build/io/io.s.o: ./io/io.s
	nasm -f elf -g ./io/io.s -o ./build/io/io.s.o

./build/mm/heap/heap.o: ./mm/heap/heap.c
	i686-elf-gcc $(INCLUDES) -I./mm/heap $(FLAGS) -std=gnu99 -c ./mm/heap/heap.c -o ./build/mm/heap/heap.o 

./build/mm/heap/kheap.o: ./mm/heap/kheap.c
	i686-elf-gcc $(INCLUDES) -I./mm/heap $(FLAGS) -std=gnu99 -c ./mm/heap/kheap.c -o ./build/mm/heap/kheap.o 

./build/mm/paging/paging.o: ./mm/paging/paging.c
	i686-elf-gcc $(INCLUDES) -I./mm/paging $(FLAGS) -std=gnu99 -c ./mm/paging/paging.c -o ./build/mm/paging/paging.o 

./build/mm/paging/paging.s.o: ./mm/paging/paging.s
	nasm -f elf -g ./mm/paging/paging.s -o ./build/mm/paging/paging.s.o

./build/drivers/disk/disk.o: ./drivers/disk/disk.c
	i686-elf-gcc $(INCLUDES) -I./drivers/disk $(FLAGS) -std=gnu99 -c ./drivers/disk/disk.c -o ./build/drivers/disk/disk.o 


./build/drivers/display/vga/vga.o: ./drivers/display/vga/vga.c
	i686-elf-gcc $(INCLUDES) -I./drivers/display/vga $(FLAGS) -std=gnu99 -c ./drivers/display/vga/vga.c -o ./build/drivers/display/vga/vga.o 

./build/fs/parser/parser.o: ./fs/parser/parser.c
	i686-elf-gcc $(INCLUDES) -I./fs/parser $(FLAGS) -std=gnu99 -c ./fs/parser/parser.c -o ./build/fs/parser/parser.o

./build/fs/vfs/vfs.o: ./fs/vfs/vfs.c
	i686-elf-gcc $(INCLUDES) -I./fs/vfs $(FLAGS) -std=gnu99 -c ./fs/vfs/vfs.c -o ./build/fs/vfs/vfs.o
	
./build/fs/fat16/fat16.o: ./fs/fat16/fat16.c
	i686-elf-gcc $(INCLUDES) -I./fs/fat16 $(FLAGS) -std=gnu99 -c ./fs/fat16/fat16.c -o ./build/fs/fat16/fat16.o
	


./build/string/string.o: ./string/string.c
	i686-elf-gcc $(INCLUDES) -I./string $(FLAGS) -std=gnu99 -c ./string/string.c -o ./build/string/string.o


./build/gdt/gdt.o: ./kernel/gdt/gdt.c
	i686-elf-gcc $(INCLUDES) -I./kernel/gdt $(FLAGS) -std=gnu99 -c ./kernel/gdt/gdt.c -o ./build/gdt/gdt.o

./build/gdt/gdt.s.o: ./kernel/gdt/gdt.s
	nasm -f elf -g ./kernel/gdt/gdt.s -o ./build/gdt/gdt.s.o

./build/task/tss/tss.s.o: ./kernel/task/tss/tss.s
	nasm -f elf -g ./kernel/task/tss/tss.s -o ./build/task/tss/tss.s.o

./build/task/task.o: ./kernel/task/task.c
	i686-elf-gcc $(INCLUDES) -I./kernel/task $(FLAGS) -std=gnu99 -c ./kernel/task/task.c -o ./build/task/task.o

./build/fs/file.o: ./fs/file.c
	i686-elf-gcc $(INCLUDES) -I./fs $(FLAGS) -std=gnu99 -c ./fs/file.c -o ./build/fs/file.o
	
./build/task/process/process.o: ./kernel/task/process/process.c
	i686-elf-gcc $(INCLUDES) -I./kernel/task/process $(FLAGS) -std=gnu99 -c ./kernel/task/process/process.c -o ./build/task/process/process.o

./build/task/task.s.o: ./kernel/task/task.s
	nasm -f elf -g ./kernel/task/task.s -o ./build/task/task.s.o

./build/idt/__0x80/__0x80.o: ./kernel/idt/__0x80/__0x80.c
	i686-elf-gcc $(INCLUDES) -I./kernel/idt/__0x80 $(FLAGS) -std=gnu99 -c ./kernel/idt/__0x80/__0x80.c -o ./build/idt/__0x80/__0x80.o
	
./build/idt/__0x80/io/io.o: ./kernel/idt/__0x80/io/io.c
	i686-elf-gcc $(INCLUDES) -I./kernel/idt/__0x80/io $(FLAGS) -std=gnu99 -c ./kernel/idt/__0x80/io/io.c -o ./build/idt/__0x80/io/io.o
	
./build/drivers/keyboard/keyboard.o: ./drivers/keyboard/keyboard.c
	i686-elf-gcc $(INCLUDES) -I./drivers/keyboard $(FLAGS) -std=gnu99 -c ./drivers/keyboard/keyboard.c -o ./build/drivers/keyboard/keyboard.o 

./build/drivers/keyboard/ps2/ps2.o: ./drivers/keyboard/ps2/ps2.c
	i686-elf-gcc $(INCLUDES) -I./drivers/keyboard/ps2 $(FLAGS) -std=gnu99 -c ./drivers/keyboard/ps2/ps2.c -o ./build/drivers/keyboard/ps2/ps2.o 

./build/g-loader/elf/elf.o: ./g-loader/elf/elf.c
	i686-elf-gcc $(INCLUDES) -I./g-loader/elf $(FLAGS) -std=gnu99 -c ./g-loader/elf/elf.c -o ./build/g-loader/elf/elf.o 

./build/g-loader/g-loader.o: ./g-loader/g-loader.c
	i686-elf-gcc $(INCLUDES) -I./g-loader $(FLAGS) -std=gnu99 -c ./g-loader/g-loader.c -o ./build/g-loader/g-loader.o 


make_programs:
	cd ./programs/yuvr && $(MAKE) all

clean_programs:
	cd ./programs/yuvr && $(MAKE) clean

clean:
	rm -rf ./bin/boot.bin	
	rm -rf ./bin/os.bin
	rm -rf ./bin/kernel.bin
	rm -rf $(FILES)
	rm -rf ./build/kernelfull.o
	rm -rf ./kernel/kernel.o