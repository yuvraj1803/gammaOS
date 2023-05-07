/*
 *
 * Copyright (c) 2023 Yuvraj Sakshith <ysakshith@gmail.com>
 *
 */


#ifndef __0x80_IO_H__
#define __0x80_IO_H__

#include "../../idt.h"

void* __0x80_PRINT(struct interrupt_frame* iframe);
void* __0x80_GETCH(struct interrupt_frame* iframe);
void* __0x80_PUTCH(struct interrupt_frame* iframe);

#endif