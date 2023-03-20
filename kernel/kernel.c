#include "kernel.h"
#include "idt/idt.h"
#include "../io/io.h"
#include "../mm/heap/kheap.h"
#include "../mm/paging/paging.h"



#define VIDEO_MEM_BASE 0xB8000
#define VGA_WIDTH      80
#define VGA_HEIGHT     80

uint16_t * VIDEO_MEM;
uint8_t cursor_x, cursor_y;

// given a character and its color, returns its hex(char:color) in little endian.
uint16_t kmake_char(char c, char color){
    return (color << 8) | c;
}

uint32_t kprintf(const char * str){
    uint32_t strlen = 0;

    while(str[strlen]){

        if(str[strlen] == '\n'){
            cursor_x = 0;
            cursor_y++;
            strlen++;
            continue;
        }

        VIDEO_MEM[cursor_y * VGA_WIDTH + cursor_x] = kmake_char(str[strlen], 0xf);
        strlen++;
        cursor_x++;


        if(cursor_x == VGA_WIDTH){
            cursor_x = 0;
            cursor_y++;
        }
    }

    return strlen;

}

void kpanic(const char* str){
    kprintf(str);
    while(1); // halt the kernel at this point.
}


void kclear_display(){
    cursor_x = 0;
    cursor_y = 0;
    for(int x=0;x<VGA_WIDTH;x++){
        for(int y=0;y<VGA_HEIGHT;y++){
            VIDEO_MEM[y*VGA_WIDTH + x] = kmake_char(' ',0);
        }
    }

}

struct vaddr_space* kernel_space = 0;

void kinit(){
    
    // initialise video memory
    VIDEO_MEM = (uint16_t*)(VIDEO_MEM_BASE);

    kclear_display(); // clear the screen
    idt_init(); // initialise interrupt descriptor table

    kheap_init(); // initialise kernel heap section

    /* creating kernel address space and loading its page directory into cr3*/
    
    struct vaddr_space* kernel_space = create_virtual_address_space(PAGE_PRESENT | PAGE_USER_ACCESS | PAGE_WRITE_ACCESS);
    load_page_directory(kernel_space->pd);

    char* p1 =  kzalloc(10);
    p1 = "yuv";

    map_vaddr_to_val(kernel_space->pd, (void*) 0x1000, (uint32_t) p1 | PAGE_PRESENT | PAGE_USER_ACCESS | PAGE_WRITE_ACCESS);


    enable_paging(); // enable paging

    char* p2 = (char*) 0x1000;
    p2[0] = 's';
    p2[1] = 'a';
    p2[2] = 'k';


    kprintf(p1);
    kprintf("\n");
    kprintf(p2);

    __enable_irq(); // enable interrupts



}