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


void kinit(){
#if defined(VGA)
    vga_init();
#endif

    // form gdt
    memset(_gdt, 0, sizeof(struct  gdt));
    get_gdt_from_structured(_gdt, _gdt_s, GDT_ENTRIES);

    // load gdt
    load_gdt(_gdt, sizeof(_gdt));

    // form tss
    memset(&_tss, 0, sizeof(_tss));
    _tss.esp0 = 0x600000;  
    _tss.ss0  = KERNEL_DATA_SELECTOR;

    // load tss
    tss_load(0x28); // 0x28 is the offset of tss in gdt


    kclear_display(); // clear the screen
    idt_init(); // initialise interrupt descriptor table
    __0x80_init(); // initialise 0x80 int

    kheap_init(); // initialise kernel heap section

    /* creating kernel address space and loading its page directory into cr3*/
    kernel_space = create_virtual_address_space(PAGE_PRESENT | PAGE_USER_ACCESS | PAGE_WRITE_ACCESS);
    if(kernel_space == 0x0){
        kpanic("[KERNEL ERROR]: Couldn't create kernel address space...\n");
    }
    load_page_directory(kernel_space->pd);

    file_init(); // initialise the virtual file system layer
    if(vfs_attach(fat16_init()) < 0){// attach fat16 fs into the primary disk
        kpanic("[KERNEL ERROR]: Couldn't attach filesystem to primary disk");
    } 

    if(!disk_init('A')){
        kpanic("[KERNEL ERROR]: Couldn't initialise primary disk...\n");
    }

    enable_paging(); // enable paging

    __enable_irq(); // enable interrupts


    struct process* p = 0;
    p = process_new("A:/yuvr.bin");

    if(!p){
        kpanic("process not created!!:(\n");
    }
        kprint_int(55);

    gammaos_first_ever_task();


    while(1){}

    // int fd = fopen("A:/yuvr.bin",'r');
    // char buf[100];
    // fread((uint8_t*)buf,100,1,fd);
    
    // kprintf(buf);

    



}