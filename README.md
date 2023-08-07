# gamma
 gamma is a lightweight multithreaded kernel built for x86 (32-bit) computers.

<img width="770" alt="gamma_test" src="https://github.com/yuvraj1803/gammaOS/assets/37622719/e9209a52-6eb8-47a3-9878-5d4c88277ebc">


# Features
 + gamma bootloader
 + VGA Display Support
 + Disk Drivers
 + PS-2 Keyboard Support
 + FAT16 Filesystem Support
 + gLoader - ELF Loader build exclusively for gammaOS
 + gShell - a super lightweight shell

# Dependencies
 + build-essential
 + bison
 + flex
 + libgmp3-dev
 + libgmp3-dev
 + libmpfr-dev
 + texinfo
 + libcloog-isl-dev
 + libisl-dev
 + **QEMU**
 + i686-elf-gcc
 + nasm

 + Refer https://wiki.osdev.org/GCC_Cross-Compiler if you find difficulty installing the cross-compiler on your machine.

 Note: Only QEMU is required for running. Rest are for building the kernel.
 
# How to use?
 + Download the build from the releases.
 + run : qemu-system-i386 -hda gamma.bin
 + This should boot up the kernel with the shell

# Contributing
 + Please feel free to send pull requests.
 + Mail me at ysakshith@gmail.com for any queries regarding the kernel.
 
