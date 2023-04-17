/*
 *
 * Copyright (c) 2023 Yuvraj Sakshith <ysakshith@gmail.com>
 *
 */

#include "gdt.h"
#include "../kernel.h"

void gdt_unstructure(uint8_t* target, struct gdt_structured source){

    // get limit is a 20 bit integer, hence we need to check for overflow.
    if((source.limit & 0xFFF00000) != 0){
        kpanic("[KERNEL ERROR]: Invalid GDT trying to be loaded. Limit larger than 20 bits.");
    }

    target[6] = 0x40;

    if(source.limit > 0xFFFF){
        source.limit >>= 12;
        target[6] = 0xC0;
    }

    // set limit
    target[0] = source.limit & 0xFF;
    target[1] = (source.limit >> 8) & 0xFF;
    target[6]|= (source.limit >> 16) & 0x0F;

    // set base
    target[2] = source.base & 0xFF;
    target[3] = (source.base >> 8) & 0xFF;
    target[4] = (source.base >> 16) & 0xFF;
    target[7] = (source.base >> 24) & 0xFF;

    // set attributes

    target[5] = source.attrubutes;


}

void get_gdt_from_structured(struct gdt* _gdt, struct gdt_structured *_gdt_s, uint16_t gdt_entries){
    for(uint16_t entry=0; entry < gdt_entries; entry++){
        gdt_unstructure((uint8_t*) &_gdt[entry], _gdt_s[entry]);
    }
}