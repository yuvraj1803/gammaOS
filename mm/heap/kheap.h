/*
 *
 * Copyright (c) 2023 Yuvraj Sakshith <ysakshith@gmail.com>
 *
 */

#ifndef __KHEAP_H__
#define __KHEAP_H__

#include <stdint.h>

void kheap_init();

void* kmalloc(uint32_t _size_in_bytes);
void* kzalloc(uint32_t _size_in_bytes);
void kfree(void* _base);

#endif