#ifndef __USRSPC_STRING_H__
#define __USRSPC_STRING_H__


unsigned int strlen(const char* str);
char* strcpy(char* tgt, const char* src);
int strncmp(const char* s1, const char* s2, unsigned int n);
int strncpy(char* tgt, char* src, unsigned int len);


#endif