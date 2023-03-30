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

struct vaddr_space* kernel_space;

struct gdt _gdt[GDT_ENTRIES];
struct gdt_structured _gdt_s[GDT_ENTRIES] = {
    {.base = 0, .limit=0x00, .attrubutes=0x00},
    {.base = 0, .limit=0x000FFFFF, .attrubutes=0x9a}, // kernel code
    {.base = 0, .limit=0x000FFFFF, .attrubutes=0x92} // kernel data
    
};

void kinit(){
#if defined(VGA)
    vga_init();
#endif

    // form gdt
    memset(_gdt, 0, sizeof(struct  gdt));
    get_gdt_from_structured(_gdt, _gdt_s, GDT_ENTRIES);

    // load gdt
    load_gdt(_gdt, sizeof(_gdt));


    kclear_display(); // clear the screen
    idt_init(); // initialise interrupt descriptor table

    kheap_init(); // initialise kernel heap section

    /* creating kernel address space and loading its page directory into cr3*/
    kernel_space = create_virtual_address_space(PAGE_PRESENT | PAGE_USER_ACCESS | PAGE_WRITE_ACCESS);
    load_page_directory(kernel_space->pd);

    disk_init();

    enable_paging(); // enable paging

    __enable_irq(); // enable interrupts

    char buf[512];

    struct disk_stream* streamer = (struct disk_stream*) kzalloc(sizeof(struct  disk_stream));
    
    disk_stream_init(streamer, 0);
    disk_stream_read(streamer, buf, 200);


    kprintf_wc("woo", 12);


}