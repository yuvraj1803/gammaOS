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

#define EOF     -1

// structure prototypes
struct disk;
struct file_stat;

typedef  int8_t (*fs_resolve)(struct disk* _disk);
typedef  void*  (*fs_open)   (struct disk* _disk, struct path* _path, uint8_t mode);
typedef  int    (*fs_read)   (struct disk* _disk, uint32_t size, uint32_t nmemb, uint8_t* dest);
typedef  int    (*fs_seek)   (struct disk* _disk, uint32_t offset, uint8_t whence);
typedef  int    (*fs_close)  (void* fd_private_data);
typedef  int    (*fs_fstat)  (void* fd_private_data, struct file_stat* stat);

struct filesystem{
    char name[10];

    fs_resolve resolve;
    fs_open    open;
    fs_read    read;
    fs_seek    seek;    
    fs_close   close;
    fs_fstat   fstat;

};

enum FSTAT_FLAGS{
    RDONLY,
    WTONLY
};

struct file_descriptor{
    uint32_t index;
    struct filesystem* fs;
    void* fs_file_descriptor; // file descriptor for that specific file system
    struct disk* disk;

    uint32_t position;
};

struct file_stat{
    uint32_t size;
    uint8_t  mode;
    uint32_t flags;

};

void vfs_init();
int8_t vfs_attach(struct filesystem* fs);
struct filesystem* vfs_resolve(struct disk* _disk);
struct file_descriptor* vfs_get_file_descriptor(uint32_t fd);

int vfs_fopen(const char* filename, uint8_t mode);
int vfs_fread(uint8_t* dest, uint32_t size, uint32_t nmemb, int fd);
int vfs_fseek(int fd, uint32_t offset, uint8_t whence);
int vfs_fclose(int fd);
int vfs_fstat(int fd, struct file_stat* stat);

#endif