/*
 *
 * Copyright (c) 2023 Yuvraj Sakshith <ysakshith@gmail.com>
 *
 */

#include "memory.h"

// starting from the base address, sets 'size' number of bytes to value provided.
void * memset(void * base_addr, int init_val, size_t size){
    char * __base_addr = (char*) base_addr;

    for(int i=0;i<size;i++){
        __base_addr[i] = (char)init_val;
    }

    return (void*)__base_addr;
}


// copies _size bytes of memory from src to tgt.
void* memcpy(void* tgt, const void* src, size_t _size){
    uint8_t* tgt_ptr = (uint8_t *) tgt;
    uint8_t* src_ptr = (uint8_t *) src;

    while(_size--){
        *tgt_ptr = *src_ptr;
        tgt_ptr++;
        src_ptr++;
    }

    return tgt;

}