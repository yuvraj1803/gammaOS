#include "kernel.h"
#include "idt/idt.h"
#include "../io/io.h"


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

void kclear_display(){
    cursor_x = 0;
    cursor_y = 0;
    for(int x=0;x<VGA_WIDTH;x++){
        for(int y=0;y<VGA_HEIGHT;y++){
            VIDEO_MEM[y*VGA_WIDTH + x] = kmake_char(' ',0);
        }
    }

}


void kinit(){
    
    VIDEO_MEM = (uint16_t*)(VIDEO_MEM_BASE);

    kclear_display(); // clear the screen
    idt_init(); // initialise interrupt descriptor table
    __enable_irq(); // enable interrupts

    kprintf("yeah, too cool right?");



}