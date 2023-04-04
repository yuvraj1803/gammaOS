#include "vfs.h"
#include "../../config.h"
#include "../../kernel/kstatus.h"


struct filesystem*      fs_list[FS_TOTAL_FILESYSTEMS];
struct file_descriptor* fd_list[FS_TOTAL_FILE_DESCRIPTORS];

void fs_init(){
    memset(fs_list, 0, sizeof(fs_list));
    memset(fd_list, 0, sizeof(fs_list));
}

struct file_descriptor* fs_get_file_descriptor(uint32_t fd){
    if(fd >= 0 && fd < FS_TOTAL_FILE_DESCRIPTORS){
        return fd_list[fd];
    }

    return 0; //invalid fd asked.
}

static struct filesystem* fs_get_free_fs(){
    for(int index=0;index < FS_TOTAL_FILESYSTEMS; index++){
        if(fs_list[index] == 0){
            return &fs_list[index];  
        }
    }

    return 0; // when all fs slots are occupied. no additional fs can be added.
}

int8_t fs_attach(struct filesystem* fs){
    struct filesystem** free_fs = fs_get_free_fs();

    if(!free_fs){
        return -ERR_FS_FAIL;
    }

    *free_fs = fs;

}

struct filesystem* fs_resolve(struct disk* _disk){

    struct filesystem* _fs;

    for(int index =0;index < FS_TOTAL_FILESYSTEMS; index++){
        if(fs_list[index] != 0 && fs_list[index]->resolve(_disk) == SUCCESS){
            _fs = fs_list[index];
            break;
        }
    }

    return _fs; // disk cannot be resolved by any filesystem.


}