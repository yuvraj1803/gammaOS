#include "kernel.h"
#include "idt/idt.h"
#include "io/io.h"


#define VIDEO_MEM_BASE 0xB8000
#define VGA_WIDTH      80
#define VGA_HEIGHT     80

uint16_t * VIDEO_MEM;
uint8_t cursor_x, cursor_y;

uint16_t kgetchx(char c, char color){
    return (color << 8) | c;
}

uint32_t kprintf(const char * str){
    uint32_t strlen = 0;

    while(str[strlen]){

        VIDEO_MEM[cursor_y * VGA_WIDTH + cursor_x] = kgetchx(str[strlen], 0xf);
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
            VIDEO_MEM[y*VGA_WIDTH + x] = kgetchx(' ',0);
        }
    }

}


void kinit(){
    
    VIDEO_MEM = (uint16_t*)(VIDEO_MEM_BASE);

    kclear_display();
    idt_init();





}