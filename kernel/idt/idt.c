/*
 *
 * Copyright (c) 2023 Yuvraj Sakshith <ysakshith@gmail.com>
 *
 */
#include "idt.h"
#include "../../config.h"
#include "../../mm/memory.h"
#include "../kernel.h"
#include "../io/io.h"
#include "../task/task.h"
#include "__0x80/__0x80.h"
#include "../../drivers/keyboard/keyboard.h"

#define _32BIT_INTERRUPT_GATE   0xE
#define _32BIT_TRAP_GATE        0xF
#define _16BIT_INTERRUPT_GATE   0x6
#define _16BIT_TRAP_GATE        0x7


#define IDT_PRESENT     1
#define IDT_RING        3
#define IDT_GATE        _32BIT_INTERRUPT_GATE

struct idt_desc idt_descriptors[TOTAL_INTERRUPTS]; // Interrupt descriptor table
struct idtr_desc idtr_descriptor; // Interrupt descriptor table register

extern void idt_load(struct idtr_desc * idtr);
extern void general_execption();
extern void isr_0x80();
extern void isr_0x21();

void* isr_0x80_handler(int command, struct interrupt_frame* iframe);

// no int handler is mapped to no_inth, called when an interrupt has not been defined yet. 
// this is done to avoid resetting the processor accidentally.
void general_execption_handler(){
    outb(0x20,0x20);
}

void idt_set(int interrupt_no, void* int_address){
    struct idt_desc* desc = &idt_descriptors[interrupt_no];
    desc->offset1 = (uint32_t) int_address & 0x0000FFFF; // low 16 bits of address
    desc->selector = KERNEL_CODE_SELECTOR;  
    desc->type_attr = (IDT_PRESENT * 0x8 + IDT_RING*0x2)*0x10 | IDT_GATE;
    desc->offset2 = (uint32_t) int_address >> 16; // high 16 bits of address
}



void idt_init(){
    memset(idt_descriptors, 0, sizeof(idt_descriptors));
    idtr_descriptor.limit = sizeof(idt_descriptors) - 1;
    idtr_descriptor.base  = (uint32_t) idt_descriptors;

    for(int _intr=0;_intr<TOTAL_INTERRUPTS;_intr++){
        idt_set(_intr, general_execption);
    }

    idt_set(0x80, isr_0x80); // for system calls 
    idt_set(0x21, isr_0x21); // for keyboard interrupts

    // tell processor where idt is. check idt.s
    idt_load(&idtr_descriptor);


}

void* isr_0x80_handler(int command, struct interrupt_frame* iframe){
    change_to_kernel_page_directory(); // switch to kernel virtual address space

    current_task_save_state(iframe);

    void* _ret = __0x80_command_handler(command, iframe);

    change_to_current_task_page_directory(); // come back to process virtual address space


    return _ret;
}