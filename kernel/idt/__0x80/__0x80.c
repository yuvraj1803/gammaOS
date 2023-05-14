/*
 *
 * Copyright (c) 2023 Yuvraj Sakshith <ysakshith@gmail.com>
 *
 */


#include "__0x80.h"
#include "../../../config.h"
#include "../../../mm/memory.h"
#include "../../kstatus.h"
#include "../../kernel.h"
#include "io/io.h"
#include "../../task/task.h"
#include "mm/heap/heap.h"

typedef void *(*__0x80_command_function)(struct interrupt_frame *iframe);

__0x80_command_function __0x80_functions[KERNEL_MAX_0x80_FUNCTIONS];

int8_t __0x80_add_function(int command, __0x80_command_function function);

void __0x80_init()
{
    memset(__0x80_functions, 0, sizeof(__0x80_functions));

    __0x80_add_function(__NR_0_PRINT, __0x80_PRINT);
    __0x80_add_function(__NR_1_GETCH, __0x80_GETCH);
    __0x80_add_function(__NR_2_PUTCH, __0x80_PUTCH);
    __0x80_add_function(__NR_3_MALLOC,__0x80_MALLOC);
    __0x80_add_function(__NR_4_FREE,  __0x80_FREE);
}


int8_t __0x80_add_function(int command, __0x80_command_function function)
{
    if (command < 0 || command >= KERNEL_MAX_0x80_FUNCTIONS)
    {
        kpanic("0x80 function couldn't be added");
    }

    if (__0x80_functions[command])
    {
        kpanic("0x80 function being overwritten!");
    }

    __0x80_functions[command] = function;

    return SUCCESS;
}

void *__0x80_command_handler(int command, struct interrupt_frame *iframe)
{

    if (command < 0 || command >= KERNEL_MAX_0x80_FUNCTIONS)
    {
        return 0; // invalid command
    }

    if (!__0x80_functions[command])
    {
        return 0; // function never added for this command.
    }

    return __0x80_functions[command](iframe);
}