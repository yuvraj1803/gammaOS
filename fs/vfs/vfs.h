#ifndef __VFS_H__
#define __VFS_H__

#include "../../drivers/disk/disk.h"
#include "../parser/parser.h"

enum{
    READ,
    WRITE,
    APPEND,
    INVALID
};

enum{
    SEEK_SET,
    SEEK_CUR,
    SEEK_END
};

struct filesystem{
    char name[10];

    int8_t (*resolve)(struct disk* _disk);

};

struct file_descriptor{
    uint32_t index;
    struct filesystem* fs;
    void* private_data;
    struct disk* disk;
};



#endif