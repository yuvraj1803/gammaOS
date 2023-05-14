#ifndef __STDLIB_H__
#define __STDLIB_H__

#include <stdint.h>


void* malloc(uint32_t size);
void free(void* base_addr);

char* itoa(int num);

#endif