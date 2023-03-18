#include "idt.h"
#include "../config.h"
#include "../memory/memory.h"
#include "../kernel.h"
#include "../io/io.h"
struct idt_desc idt_descriptors[TOTAL_INTERRUPTS];
struct idtr_desc idtr_descriptor;

extern void idt_load(void * ptr);
extern void int21h();
extern void no_inth();

void idt_zero(){
    kprintf("dividing by zero not allowed");
    return;
}

void int21_handler(){
    kprintf("keyboard is pressed bb");

    outb(0x20,0x20);
}

void no_int_handler(){

    outb(0x20,0x20);
}

void idt_set(int interrupt_no, void * address){
    struct idt_desc* desc = &idt_descriptors[interrupt_no];
    desc->offset1 = (uint32_t) address & 0x0000FFFF;
    desc->selector = KERNEL_CODE_SELECTOR;  
    desc->type_attr = 0xEE;
    desc->offset2 = (uint32_t) address >> (1 << 4);
}

void idt_init(){
    memset(idt_descriptors, 0, sizeof(idt_descriptors));
    idtr_descriptor.limit = sizeof(idt_descriptors) - 1;
    idtr_descriptor.base  = (uint32_t) idt_descriptors;

    for(int i=0;i<TOTAL_INTERRUPTS;i++){
        idt_set(i, no_inth);
    }


    idt_set(0, idt_zero);
    idt_set(0x21, int21h);

    // tell processor where idt is. check idt.s
    idt_load(&idtr_descriptor);
}