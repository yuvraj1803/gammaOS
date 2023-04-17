#ifndef __FILE_H__
#define __FILE_H__

#include "vfs/vfs.h"

int     fopen(const char* filename, uint8_t mode);
int     fread(uint8_t* dest, uint32_t size, uint32_t nmemb, int fd);
int     fseek(int fd, uint32_t offset, uint8_t whence);
int     fclose(int fd);
int     fstat(int fd, struct file_stat* stat);
void    file_init();

#endif