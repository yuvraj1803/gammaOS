#include "vga.h"


#define VGA_TEXT_BUFFER     0xB8000
#define VGA_WIDTH           80
#define VGA_HEIGHT          80

uint16_t * VGA_BASE; // base address of vga text buffer
uint8_t cursor_x, cursor_y; // current positions on the screen.


void vga_init(){
    VGA_BASE = (uint16_t*) VGA_TEXT_BUFFER;
    cursor_x = 0;
    cursor_y = 0;
}

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

        VGA_BASE[cursor_y * VGA_WIDTH + cursor_x] = kmake_char(str[strlen], 0xf);
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
            VGA_BASE[y*VGA_WIDTH + x] = kmake_char(' ',0);
        }
    }

}