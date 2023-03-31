#ifndef __TSS_H__
#define __TSS_H__

#include <stdint.h>
struct tss
{
    uint32_t link;
    uint32_t esp0;  // kernel sp
    uint32_t ss0;   // kernel ss
    uint32_t esp1;
    uint32_t esp2;
    uint32_t ss2;
    uint32_t sr3;
    uint32_t eip;
    uint32_t eflags;
    uint32_t eax;
    uint32_t ecx;
    uint32_t edx;
    uint32_t ebx;
    uint32_t esp;
    uint32_t ebp;
    uint32_t esi;
    uint32_t edi;
    uint32_t es;
    uint32_t cs;
    uint32_t ss;
    uint32_t ds;
    uint32_t fs;
    uint32_t gs;
    uint32_t ldtr;
    uint32_t iopb;
} __attribute__((packed));

void tss_load(uint16_t tss_segment_base);

#endif