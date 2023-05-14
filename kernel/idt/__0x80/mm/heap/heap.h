#ifndef __0x80_HEAP_H
#define __0x80_HEAP_H

#include "../../../idt.h"

void* __0x80_MALLOC(struct interrupt_frame* iframe);
void*  __0x80_FREE  (struct interrupt_frame* iframe);


#endif