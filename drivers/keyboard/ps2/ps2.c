/*
 *
 * Copyright (c) 2023 Yuvraj Sakshith <ysakshith@gmail.com>
 *
 */

#include "ps2.h"
#include "../keyboard.h"
#include <stdint.h>
#include "../../../io/io.h"
#include "../../../kernel/kstatus.h"
#include "../../../mm/paging/paging.h"
#include "../../../kernel/kernel.h"
#include "../../../kernel/task/task.h"
#include "../../../kernel/idt/idt.h"

//useful macros
#define SIZEOF_ARRAY(arr) sizeof(arr)/sizeof(arr[0])


#define PS2_DATA_PORT           0x60
#define PS2_COMMAND_PORT        0x64
#define PS2_ENABLE_FIRST_PORT   0xAE
#define PS2_KEY_RELEASED        0x80
#define PS2_RETURN              0x1C
#define PS2_BACKSPACE           0x0E
#define PS2_SPACE               0x39
#define PS2_CAPS                0x3A


static uint8_t ps2_scancode_set1[] = {
    0x00, 0x1B, '1', '2', '3', '4', '5',
    '6', '7', '8', '9', '0', '-', '=',
    0x08, '\t', 'Q', 'W', 'E', 'R', 'T',
    'Y', 'U', 'I', 'O', 'P', '[', ']',
    0x0d, 0x00, 'A', 'S', 'D', 'F', 'G',
    'H', 'J', 'K', 'L', ';', '\'', '`', 
    0x00, '\\', 'Z', 'X', 'C', 'V', 'B',
    'N', 'M', ',', '.', '/', 0x00, '*',
    0x00, 0x20, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, '7', '8', '9', '-', '4', '5',
    '6', '+', '1', '2', '3', '0', '.'
};

int ps2_keyboard_init();
void ps2_interrupt_handler();

struct keyboard ps2 = {
    .name = "PS/2",
    .init = ps2_keyboard_init,
    .next = 0
};

int ps2_keyboard_init(){

    // enable ps2 first port.
    outb(PS2_COMMAND_PORT, PS2_ENABLE_FIRST_PORT);

    ps2.caps_lock = 0; // initially turn off the caps lock.

    idt_add_isr_handler(0x21, ps2_interrupt_handler);

    return SUCCESS;
}

struct keyboard* ps2_init(){
    return &ps2;
}

char ps2_scancode_to_char(uint8_t scancode){

    if(scancode < 0 || scancode >= SIZEOF_ARRAY(ps2_scancode_set1)){
        return -ERR_INVARG;
    }

    return ps2_scancode_set1[scancode];

}

void ps2_interrupt_handler(){

    change_to_kernel_page_directory();

    uint32_t scancode = insb(PS2_DATA_PORT);

    if(scancode & PS2_KEY_RELEASED){
        return;
    }

    if(scancode == PS2_CAPS){
        ps2.caps_lock ^= 1;// turn on the caps lock flag.
    }

    if(scancode == PS2_RETURN){
        keyboard_put('\n');
    }else{
        char input_char = ps2_scancode_to_char(scancode);

        if(!ps2.caps_lock && scancode != PS2_BACKSPACE && scancode != PS2_SPACE && input_char >= 'A' && input_char <= 'Z'){ //if caps lock off.
            input_char += 32; // convert to lower-case.
        }

        keyboard_put(input_char);
    }

    // if there is no task running, there will be a garbage value loaded into cr3.
    // great way to break your computer.
    if(current_task()) change_to_current_task_page_directory();

    

}