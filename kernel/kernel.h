/*
 *
 * Copyright (c) 2023 Yuvraj Sakshith <ysakshith@gmail.com>
 *
 */

#ifndef __KERNEL_H__
#define __KERNEL_H__

#include <stdint.h>

void init();
uint32_t kputs(const char * str);
void kprint_int(int num);
void kpanic(const char* str);
void set_all_segments_to_user_data_segment();
void set_all_segments_to_kernel_data_segment();
void change_to_kernel_page_directory();


#endif