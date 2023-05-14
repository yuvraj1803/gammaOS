/*
 *
 * Copyright (c) 2023 Yuvraj Sakshith <ysakshith@gmail.com>
 *
 */

#include "vga.h"


#define VGA_TEXT_BUFFER     0xB8000
#define VGA_WIDTH           80
#define VGA_HEIGHT          80

#define PS2_BACKSPACE       0x08
#define PS2_RETURN          0xA

uint16_t * VGA_BASE; // base address of vga text buffer
uint8_t cursor_x, cursor_y; // current positions on the screen.


void kbackspace();
void kreturn();

void vga_init(){
    VGA_BASE = (uint16_t*) VGA_TEXT_BUFFER;
    cursor_x = 0;
    cursor_y = 0;
}

// given a character and its color, returns its hex(char:color) in little endian.
uint16_t kmake_char(char c, char color){
    return (color << 8) | c;
}

uint32_t kputs(const char * str){
    uint32_t strpos = 0;

    while(str[strpos]){

        if(str[strpos] == '\n'){
            cursor_x = 0;
            cursor_y++;
            strpos++;
            continue;
        }

        kputchar(str[strpos]);

        strpos++;
    }

    return strpos;

}

// prints char into the screen.
void kputchar(char c){

    if(c == PS2_BACKSPACE){
        kbackspace();
        return;
    }

    if(c == PS2_RETURN){
        kreturn(); // pressing the return/enter key
        return;
    }

    VGA_BASE[cursor_y * VGA_WIDTH + cursor_x] = kmake_char(c, 0xf);
    cursor_x++;

    if(cursor_x == VGA_WIDTH){
        cursor_x = 0;
        cursor_y++;
    }
}


void kbackspace(){

    // nothing to backspace
    if(cursor_x == 0 && cursor_y == 0){
        return;
    }

    cursor_x = (cursor_x - 1)%VGA_WIDTH;
    kputchar(' ');
    cursor_x = (cursor_x - 1)%VGA_WIDTH;


}

void kreturn(){
    kputs("\n");
    return;
}


void kprint_int(int num) {
    char buf[32];
    int i = 0;
    if (num == 0) {
        kputchar('0');
        return;
    }
    if (num < 0) {
        kputchar('-');
        num = -num;
    }
    while (num > 0) {
        buf[i++] = num % 10 + '0';
        num /= 10;
    }
    while (i > 0) {
        kputchar(buf[--i]);
    }
}


// kprintf with color
uint32_t kprintf_wc(const char * str, uint8_t color){
    uint32_t strlen = 0;

    while(str[strlen]){

        if(str[strlen] == '\n'){
            cursor_x = 0;
            cursor_y++;
            strlen++;
            continue;
        }

        VGA_BASE[cursor_y * VGA_WIDTH + cursor_x] = kmake_char(str[strlen], color);
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
    kputs(str);
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