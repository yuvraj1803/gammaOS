#include "vfs.h"
#include "../../config.h"
#include "../../kernel/kstatus.h"
#include "../../mm/memory.h"
#include "../parser/parser.h"
#include "../path.h"
#include "../../drivers/disk/disk.h"


struct filesystem*      fs_list[FS_TOTAL_FILESYSTEMS];
struct file_descriptor* fd_list[FS_TOTAL_FILE_DESCRIPTORS];

void vfs_init(){
    memset(fs_list, 0, sizeof(fs_list));
    memset(fd_list, 0, sizeof(fs_list));
}

struct file_descriptor* vfs_get_file_descriptor(uint32_t fd){
    if(fd >= 0 && fd < FS_TOTAL_FILE_DESCRIPTORS){
        return fd_list[fd];
    }

    return 0; //invalid fd asked.
}

struct file_descriptor* vfs_new_file_descriptor(){
    for(int index=0;index < FS_TOTAL_FILE_DESCRIPTORS;index++){
        if(fd_list[index] == 0){
            fd_list[index]->index = index;
            return fd_list[index];
        }
    }

    return 0;
}

static struct filesystem** vfs_get_free_fs(){
    for(int index=0;index < FS_TOTAL_FILESYSTEMS; index++){
        if(fs_list[index] == 0){
            return &fs_list[index];  
        }
    }

    return 0; // when all fs slots are occupied. no additional fs can be added.
}

int8_t vfs_attach(struct filesystem* fs){
    struct filesystem** free_fs = vfs_get_free_fs();

    if(!free_fs){
        return -ERR_FS_FAIL;
    }

    *free_fs = fs;

    return SUCCESS;

}

struct filesystem* vfs_resolve(struct disk* _disk){

    struct filesystem* _fs;

    for(int index =0;index < FS_TOTAL_FILESYSTEMS; index++){
        if(fs_list[index] != 0 && fs_list[index]->resolve(_disk) == SUCCESS){
            _fs = fs_list[index];
            return _fs;
        }
    }

    return 0; // disk cannot be resolved by any filesystem.

}

int vfs_fopen(const char* filename, uint8_t mode){
    if(mode == INVALID) return -ERR_FS_INV_FILE_MODE;

    struct path* file_path = parse_path(filename);
    char drive_id = file_path->drive_id;

    struct disk* _disk = disk_get(drive_id - 'A');

    void* file_data = _disk->fs->open(_disk, file_path, mode);

    if(file_data == 0){
        return -ERR_FS_FOPEN_FAILED;
    }

    struct file_descriptor* fd = vfs_new_file_descriptor();
    if(fd == 0){
        // couldn't find file descriptor
        return -ERR_FS_FOPEN_FAILED;
    }
    fd->disk = _disk;
    fd->fs   = _disk->fs;
    fd->private_data = file_data;   

    return fd->index;
}