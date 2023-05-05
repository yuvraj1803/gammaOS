/*
 *
 * Copyright (c) 2023 Yuvraj Sakshith <ysakshith@gmail.com>
 *
 */
#ifndef __IDT_H__
#define __IDT_H__

#include <stdint.h>

void idt_init();

extern void __enable_irq();
extern void __disable_irq();

struct interrupt_frame; 

typedef void (*isr_handler_function)(struct interrupt_frame* iframe);

int idt_add_isr_handler(int interrupt_no, isr_handler_function handler);

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

struct interrupt_frame
{
    uint32_t edi;
    uint32_t esi;
    uint32_t ebp;
    uint32_t useless_esp; // this is esp pushed by pushad instruction. we dont really need this for anything.
    uint32_t ebx;
    uint32_t edx;
    uint32_t ecx;
    uint32_t eax;
    uint32_t ip;
    uint32_t cs;
    uint32_t flags;
    uint32_t esp;
    uint32_t ss;
} __attribute__((packed));

#endif