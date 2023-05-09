#ifndef __G_LOADER_H__
#define __G_LOADER_H__

#include "../config.h"
#include <stdint.h>

typedef struct{

    char filename[FS_MAX_PATH_LENGTH];
    uint32_t size; // file size in bytes

    void* data; // file data


    // base and end addresses in process virtual memory
    void* vbase;
    void* vend;

    // base and end addresses in physical memory
    void* pbase;
    void* pend;


} ELF_FILE;

#endif