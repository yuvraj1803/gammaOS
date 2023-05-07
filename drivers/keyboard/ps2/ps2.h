/*
 *
 * Copyright (c) 2023 Yuvraj Sakshith <ysakshith@gmail.com>
 *
 */


#ifndef __PS2_H__
#define __PS2_H__

#include "../keyboard.h"

struct keyboard* ps2_init();
void ps2_interrupt_handler();


#endif