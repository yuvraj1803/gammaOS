#include "file.h"
#include "vfs/vfs.h"

void file_init(){
    vfs_init();
}

int fopen(const char* filename, uint8_t mode){
    return vfs_fopen(filename, mode);
}

int fread(uint8_t* dest, uint32_t size, uint32_t nmemb, int fd){
    return vfs_fread(dest,size,nmemb,fd);
}

int fseek(int fd, uint32_t offset, uint8_t whence){
    return vfs_fseek(fd, offset,whence);
}

int fclose(int fd){
    return vfs_fclose(fd);
}

int fstat(int fd, struct file_stat* stat){
    return vfs_fstat(fd,stat);
}
