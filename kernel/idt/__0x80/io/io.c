#include "io.h"
#include "../../../kernel.h"
#include "../../../kstatus.h"
#include "../../../task/task.h"
#include "../../../../drivers/keyboard/keyboard.h"
#include "../../../../drivers/display/vga/vga.h"


void* __0x80_PRINT(struct interrupt_frame* iframe){
    void* arg = task_access_stack(current_task(), 0); // string passed.

    // the address of the string that was passed to us was in the task's address space.
    // we need to send it to the kernel to be able to print it.

    char arg_in_kernel_space[1024];
    copy_data_from_task_to_kernel(current_task(), arg, (void*) arg_in_kernel_space, sizeof(arg_in_kernel_space));

    kprintf(arg_in_kernel_space);

    return (void*) SUCCESS;
}

void* __0x80_GETCH(struct interrupt_frame* iframe){
    void* ch = (void*)((int)keyboard_get());

    return ch;

}

void* __0x80_PUTCH(struct interrupt_frame* iframe){

    char ch = (char)(int)task_access_stack(current_task(), 0);

    kputchar(ch);

    return (void*) SUCCESS;

}