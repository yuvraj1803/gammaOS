/*
 *
 * Copyright (c) 2023 Yuvraj Sakshith <ysakshith@gmail.com>
 *
 */

#ifndef __KERNEL_H__
#define __KERNEL_H__

#include <stdint.h>

void init();
uint32_t kprintf(const char * str);
void kpanic(const char* str);

#endif