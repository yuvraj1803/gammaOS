#include "ps2.h"
#include "../keyboard.h"
#include <stdint.h>
#include "../../../io/io.h"
#include "../../../kernel/kstatus.h"
#include "../../../mm/paging/paging.h"
#include "../../../kernel/kernel.h"
#include "../../../kernel/task/task.h"

//useful macros
#define SIZEOF_ARRAY(arr) sizeof(arr)/sizeof(arr[0])


#define PS2_DATA_PORT           0x60
#define PS2_COMMAND_PORT        0x64
#define PS2_ENABLE_FIRST_PORT   0xAE
#define PS2_KEY_RELEASED        0x80


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

struct keyboard ps2 = {
    .name = "PS/2",
    .init = ps2_keyboard_init,
    .next = 0
};

int ps2_keyboard_init(){

    // enable ps2 first port.
    outb(PS2_COMMAND_PORT, PS2_ENABLE_FIRST_PORT);

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

    uint8_t scancode = insb(PS2_DATA_PORT);

    if(scancode & PS2_KEY_RELEASED){
        // not doing anything for now
        return;
    }

    char input_char = ps2_scancode_to_char(scancode);

    keyboard_put(input_char);

    change_to_current_task_page_directory();
    

}