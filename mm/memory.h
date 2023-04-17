#ifndef __MEMORY_H__
#define __MEMORY_H__

#include <stddef.h>
#include <stdint.h>

void * memset(void * base_addr, int init_val, size_t size);
void* memcpy(void* tgt, const void* src, size_t _size);


#endif