#ifndef __0x80_H__
#define __0x80_H__

#include "../idt.h"

typedef void*(*__0x80_command_function)(struct interrupt_frame* iframe);

enum __0x80_COMMANDS{
    __NR_0_PRINT
};


void* __0x80_command_handler(int command, struct interrupt_frame *iframe);
void  __0x80_init();
int8_t __0x80_add_function(int command, __0x80_command_function function);

#endif