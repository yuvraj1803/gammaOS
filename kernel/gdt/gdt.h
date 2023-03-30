#ifndef __GDT_H__
#define __GDT_H__

#include <stdint.h>

struct gdt{
    uint16_t limit_0_15;
    uint16_t base_0_15;
    uint8_t  base_16_23;
    uint8_t access;
    uint8_t flags;
    uint8_t base_24_31;
} __attribute__((packed));

struct gdt_structured{
    uint32_t base;
    uint32_t limit;
    uint8_t  attrubutes;
}__attribute__((packed));

void get_gdt_from_structured(struct gdt* _gdt, struct gdt_structured *_gdt_s, uint16_t gdt_entries);
void load_gdt(struct gdt* _gdt, uint32_t gdt_size);

#endif