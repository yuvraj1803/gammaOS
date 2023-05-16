/*
 *
 * Copyright (c) 2023 Yuvraj Sakshith <ysakshith@gmail.com>
 *
 */

#include "kernel.h"
#include "idt/idt.h"
#include "../io/io.h"
#include "../mm/heap/kheap.h"
#include "../mm/paging/paging.h"
#include "../drivers/disk/disk.h"
#include "../drivers/display/vga/vga.h"
#include "../config.h"
#include "../fs/parser/parser.h"
#include "../fs/path.h"
#include "gdt/gdt.h"
#include "../mm/memory.h"
#include "task/tss/tss.h"
#include "../fs/file.h"
#include "../fs/fat16/fat16.h"
#include "kstatus.h"
#include "task/process/process.h"
#include "task/task.h"
#include "idt/__0x80/__0x80.h"
#include "../drivers/keyboard/keyboard.h"

struct vaddr_space* kernel_space;
struct tss _tss;
struct gdt _gdt[GDT_ENTRIES];
struct gdt_structured _gdt_s[GDT_ENTRIES] = {
    {.base = 0, .limit=0x00, .attrubutes=0x00},
    {.base = 0, .limit=0x000FFFFF, .attrubutes=0x9a}, // kernel code
    {.base = 0, .limit=0x000FFFFF, .attrubutes=0x92}, // kernel data
    {.base = 0, .limit=0x000FFFFF, .attrubutes=0xf8}, // user code
    {.base = 0, .limit=0x000FFFFF, .attrubutes=0xf2}, // user data
    {.base = (uint32_t)&_tss, .limit=sizeof(struct tss), .attrubutes=0xe9} // tss
    
};

void change_to_kernel_page_directory(){
    set_all_segments_to_kernel_data_segment();
    load_page_directory(kernel_space->pd);
}

void kernel_start(){

    kclear_display(); // clear the screen

    __disable_irq();

#if defined(VGA)
    vga_init();
#endif

    // form gdt
    memset(_gdt, 0, sizeof(struct  gdt));
    get_gdt_from_structured(_gdt, _gdt_s, GDT_ENTRIES);

    kputs("[KERNEL] setting up gdt.\n");

    // load gdt
    load_gdt(_gdt, sizeof(_gdt));

    kputs("[KERNEL] loaded up gdt.\n");

    // form tss
    memset(&_tss, 0, sizeof(_tss));
    _tss.esp0 = 0x600000;  
    _tss.ss0  = KERNEL_DATA_SELECTOR;

    kputs("[KERNEL] setting up tss.\n");


    // load tss
    tss_load(0x28); // 0x28 is the offset of tss in gdt

    kputs("[KERNEL] loaded up tss.\n");


    idt_init(); // initialise interrupt descriptor table

    kputs("[KERNEL] initialised idt. \n");

    __0x80_init(); // initialise 0x80 int

    kputs("[KERNEL] initialised gammaOS system call handlers. \n");

    kheap_init(); // initialise kernel heap section

    kputs("[KERNEL] initialised kernel heap. \n");

    keyboard_init();// intialise the abstract keyboard driver

    kputs("[KERNEL] initialised keyboard drivers. \n");

    /* creating kernel address space and loading its page directory into cr3*/
    kernel_space = create_virtual_address_space(PAGE_PRESENT | PAGE_USER_ACCESS | PAGE_WRITE_ACCESS);
    if(kernel_space == 0x0){
        kpanic("[KERNEL ERROR]: Couldn't create kernel address space...\n");
    }

    kputs("[KERNEL] initialised kernel address space. \n");

    load_page_directory(kernel_space->pd);

    kputs("[KERNEL] loaded kernel page directory. \n");

    file_init(); // initialise the virtual file system layer

    kputs("[KERNEL] initialised virtual filesystem. \n");

    if(vfs_attach(fat16_init()) < 0){// attach fat16 fs into the primary disk
        kpanic("[KERNEL ERROR]: Couldn't attach filesystem to primary disk");
    } 

    kputs("[KERNEL] attached fat16 driver. \n");

    if(!disk_init('A')){
        kpanic("[KERNEL ERROR]: Couldn't initialise primary disk...\n");
    }

    kputs("[KERNEL] initialised primary disk. \n");

    enable_paging(); // enable paging

    kputs("[KERNEL] initialised paging. \n");


    // set-up gshell
    struct process* proc_gshell = process_new("A:/usrspc/gshell/main.o");

    kputs("[KERNEL] loading gshell...\n");

    if(!proc_gshell){
        kpanic("process not created!!:(\n");
    }   

    __enable_irq(); // enable interrupts

    kclear_display();

    gammaos_first_ever_task(); // run the shell



    while(1){}
    



}