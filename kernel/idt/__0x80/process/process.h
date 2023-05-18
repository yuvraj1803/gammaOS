#ifndef __0x80_PROCESS_H__
#define __0x80_PROCESS_H__

#include "../../idt.h"

void* __0x80_EXEC(struct interrupt_frame* iframe);
void* __0x80_EXIT(struct interrupt_frame* iframe);


#endif