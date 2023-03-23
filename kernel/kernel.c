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


struct vaddr_space* kernel_space;

void kinit(){
    
#if defined(VGA)
    vga_init();
#endif

    kclear_display(); // clear the screen
    idt_init(); // initialise interrupt descriptor table

    kheap_init(); // initialise kernel heap section

    /* creating kernel address space and loading its page directory into cr3*/
    kernel_space = create_virtual_address_space(PAGE_PRESENT | PAGE_USER_ACCESS | PAGE_WRITE_ACCESS);
    load_page_directory(kernel_space->pd);


    enable_paging(); // enable paging

    __enable_irq(); // enable interrupts

    kprintf_wc("woo", 12);


}