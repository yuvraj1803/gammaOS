#ifndef __VFS_H__
#define __VFS_H__

#include "../../drivers/disk/disk.h"
#include "../parser/parser.h"

enum{
    READ    = 'r',
    WRITE   = 'w',
    APPEND  = 'a',
    INVALID
};

enum{
    SEEK_SET,
    SEEK_CUR,
    SEEK_END
};
struct disk;
typedef  int8_t (*fs_resolve)(struct disk* _disk);
typedef  void*  (*fs_open)   (struct disk* _disk, struct path* _path, uint8_t mode);

struct filesystem{
    char name[10];

    fs_resolve resolve;
    fs_open    open;

};

struct file_descriptor{
    uint32_t index;
    struct filesystem* fs;
    void* private_data;
    struct disk* disk;
};

void vfs_init();
int8_t vfs_attach(struct filesystem* fs);
struct filesystem* vfs_resolve(struct disk* _disk);
int vfs_fopen(const char* filename, uint8_t mode);
struct file_descriptor* vfs_get_file_descriptor(uint32_t fd);


#endif