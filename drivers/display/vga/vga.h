/*
 *
 * Copyright (c) 2023 Yuvraj Sakshith <ysakshith@gmail.com>
 *
 */

#ifndef __VGA_H__
#define __VGA_H__

#include <stdint.h>

void vga_init();
uint32_t kprintf(const char * str);
void kpanic(const char* str);
void kclear_display();
uint32_t kprintf_wc(const char * str, uint8_t color);
void kprint_int(int num);
void kputchar(char c);


#endif