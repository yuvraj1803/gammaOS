#ifndef __IDT_H__
#define __IDT_H__

#include <stdint.h>

void idt_init();
// interrupt descriptor table entry structure
struct idt_desc{
    uint16_t offset1;
    uint16_t selector;
    uint8_t zero;
    uint8_t type_attr;
    uint16_t offset2;
} __attribute__((packed));


// interrupt descriptor table register structure
struct idtr_desc{
    uint16_t limit;
    uint32_t base;
} __attribute__((packed));

#endif