#include "memory.h"

// starting from the base address, sets 'size' number of bytes to value provided.
void * memset(void * base_addr, int init_val, size_t size){
    char * __base_addr = (char*) base_addr;

    for(int i=0;i<size;i++){
        __base_addr[i] = (char)init_val;
    }

    return base_addr;
}