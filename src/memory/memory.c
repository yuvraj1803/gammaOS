#include "memory.h"


void * memset(void * ptr, int c, size_t size){
    char * cptr = (char*) ptr;

    for(int i=0;i<size;i++){
        cptr[i] = (char)c;
    }

    return ptr;
}