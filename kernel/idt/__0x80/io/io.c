/*
 *
 * Copyright (c) 2023 Yuvraj Sakshith <ysakshith@gmail.com>
 *
 */


#include "io.h"
#include "../../../kernel.h"
#include "../../../kstatus.h"
#include "../../../task/task.h"
#include "../../../../drivers/keyboard/keyboard.h"
#include "../../../../drivers/display/vga/vga.h"


// syscall 0
void* __0x80_PRINT(struct interrupt_frame* iframe){
    void* arg = task_access_stack(current_task(), 0); // string passed.

    // the address of the string that was passed to us was in the task's address space.
    // we need to send it to the kernel to be able to print it.

    char arg_in_kernel_space[1024];
    copy_data_from_task_to_kernel(current_task(), arg, (void*) arg_in_kernel_space, sizeof(arg_in_kernel_space));

    kputs(arg_in_kernel_space);

    return (void*) SUCCESS;
}


// syscall 1
void* __0x80_GETCH(struct interrupt_frame* iframe){
    void* ch = (void*)((int)keyboard_get());

    if(ch == 0x0) return ch;

#ifdef PRINT_KEYBOARD_INP_TO_SCREEN
    kputchar((char)(uint32_t)ch); // to know what we are typing
#endif


    return ch;

}

// syscall 2
void* __0x80_PUTCH(struct interrupt_frame* iframe){

    char ch = (char)(int)task_access_stack(current_task(), 0);

    kputchar(ch);

    return (void*) SUCCESS;

}