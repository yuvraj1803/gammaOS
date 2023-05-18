
/*
 *
 * Copyright (c) 2023 Yuvraj Sakshith <ysakshith@gmail.com>
 *
 */

#ifndef __0x80_H__
#define __0x80_H__

#include "../idt.h"

typedef void*(*__0x80_command_function)(struct interrupt_frame* iframe);

enum __0x80_COMMANDS{
    __NR_0_PRINT, 
    __NR_1_GETCH,
    __NR_2_PUTCH,
    __NR_3_MALLOC,
    __NR_4_FREE,
    __NR_5_EXEC,
    __NR_6_EXIT
    
};


void* __0x80_command_handler(int command, struct interrupt_frame *iframe);
void  __0x80_init();
int8_t __0x80_add_function(int command, __0x80_command_function function);

#endif