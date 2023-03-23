#include "string.h"


// returns the length of a given string
uint32_t strlen(const char* str){
    uint32_t strpos = 0;
    while(str[strpos] != 0x00){
        strpos++;
    }

    return strpos;
}

// copies string from src to tgt
char* strcpy(char* tgt, const char* src){
    uint32_t tgt_ix = 0;
    uint32_t src_ix = 0;

    while(src[src_ix] != 0x00){
        tgt[tgt_ix++] = src[src_ix++];
    }
    tgt[tgt_ix] = 0x00; // mark the end of the copied string

    return tgt;

}
