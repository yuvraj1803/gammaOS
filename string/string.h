#ifndef __STRING_H__
#define __STRING_H__

#include <stdint.h>

uint32_t strlen(const char* str);
char* strcpy(char* tgt, const char* src);
int strncmp(const char* s1, const char* s2, uint32_t n);

#endif