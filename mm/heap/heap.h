#ifndef __HEAP_H__
#define __HEAP_H__

#include "../config.h"
#include <stdint.h>


#define size_t uint32_t


#define HEAP_BLOCK_ENTRY_FREE   0x0
#define HEAP_BLOCK_ENTRY_TAKEN  0x1

#define HEAP_BLOCK_HAS_NEXT     0b10000000
#define HEAP_BLOCK_IS_FIRST     0b01000000

typedef uint8_t hb_entry; // each entry in heap block entry table

struct hb_table{
    hb_entry * entries;
    uint32_t num_entries; // number of entries in heap block entry table

};

struct heap{
    struct hb_table * entry_table;
    void* heap_base;
};


int32_t create_heap(struct heap* _heap, void* saddr, void* eaddr, struct hb_table* _hb_table);
void* malloc(struct heap* heap, size_t _size_in_bytes);
void free(struct heap* heap, void* _base);

#endif