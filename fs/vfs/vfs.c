/*
 *
 * Copyright (c) 2023 Yuvraj Sakshith <ysakshith@gmail.com>
 *
 */
#include "vfs.h"
#include "../../config.h"
#include "../../kernel/kstatus.h"
#include "../../mm/memory.h"
#include "../parser/parser.h"
#include "../path.h"
#include "../../drivers/disk/disk.h"
#include "../../kernel/kernel.h"
#include "../../mm/heap/kheap.h"

struct filesystem*      fs_list[FS_TOTAL_FILESYSTEMS];
struct file_descriptor* fd_list[FS_TOTAL_FILE_DESCRIPTORS];

void vfs_init(){
    
    memset(fs_list, 0, sizeof(fs_list));
    memset(fd_list, 0, sizeof(fd_list));
    
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
            
            struct file_descriptor* new_fd = (struct file_descriptor*) kzalloc(sizeof(struct file_descriptor));
            new_fd->index = index;

            fd_list[index] = new_fd;

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

    struct disk* _disk = disk_get(drive_id);

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
    fd->fs_file_descriptor = file_data;   

    return fd->index;
}

int vfs_fread(uint8_t* dest, uint32_t size, uint32_t nmemb, int fd){

    if(fd < 0 || fd >= FS_TOTAL_FILE_DESCRIPTORS){
        return -ERR_INVARG;
    }

    struct file_descriptor* _fd = vfs_get_file_descriptor(fd);

    if(_fd == 0){
        return -ERR_FD_NOT_FOUND;
    }


    _fd->fs->read(_fd->disk,_fd->fs_file_descriptor,size, nmemb, dest);

    return SUCCESS;

}

int vfs_fseek(int fd, uint32_t offset, uint8_t whence){
    struct file_descriptor* _fd = vfs_get_file_descriptor(fd);

    if(_fd == 0){
        return -ERR_FD_NOT_FOUND;
    }

    _fd->fs->seek(_fd->fs_file_descriptor, offset, whence);


    return SUCCESS;
}

int vfs_fclose(int fd){

    struct file_descriptor* _fd = vfs_get_file_descriptor(fd);

    if(_fd == 0){
        return -ERR_FD_NOT_FOUND;
    }

    fd_list[_fd->index] = 0;
    _fd->fs->close(_fd->fs_file_descriptor);


    return SUCCESS;

}

int vfs_fstat(int fd, struct file_stat* stat){
    struct file_descriptor* _fd = vfs_get_file_descriptor(fd);
    
    if(_fd == 0){
        return -ERR_FD_NOT_FOUND;
    }

    _fd->fs->stat(_fd->fs_file_descriptor, stat);

    return SUCCESS;

}