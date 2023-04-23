/*
 *
 * Copyright (c) 2023 Yuvraj Sakshith <ysakshith@gmail.com>
 *
 */

#include "heap.h"
#include "kstatus.h"
#include "../../kernel/kernel.h"
#include "../memory.h"


void* malloc_blocks(struct heap*_heap, uint32_t blocks_to_allocate);
void free_blocks(struct heap* _heap, uint32_t start_block);
static uint32_t get_aligned_heap_size(size_t _size);
static uint8_t validate_heap_alignment(void* addr);
static int validate_heap_table(void* saddr, void* eaddr, struct hb_table * _hb_table);
uint32_t find_free_entry(struct heap* _heap, uint32_t blocks_to_allocate);
void mark_blocks_taken(struct heap* _heap, uint32_t sblock, uint32_t eblock);

// creates a heap
int32_t create_heap(struct heap* _heap, void* saddr, void* eaddr, struct hb_table* _hb_table){
    // return val < 0 denotes failure.

    if(!validate_heap_alignment(saddr) || !validate_heap_alignment(eaddr)){
        return -ERR_INVARG;
    }

    if(!validate_heap_table(saddr, eaddr,_hb_table)){
        return -ERR_INVARG;
    }
    
    memset(_heap, 0, sizeof(struct heap));
    _heap->heap_base = saddr;
    _heap->entry_table = _hb_table;


    uint32_t hb_table_size = sizeof(hb_entry) * _hb_table->num_entries;

    memset(_hb_table->entries, HEAP_BLOCK_ENTRY_FREE, hb_table_size);

    return 0;
}

// malloc: allocates memory in the provided heap.

void* malloc(struct heap* _heap, size_t _size_in_bytes){
    size_t aligned_size = get_aligned_heap_size(_size_in_bytes);
    uint32_t blocks_to_allocate = aligned_size / KERNEL_HB_SIZE;

    void* malloced_base_address = malloc_blocks(_heap, blocks_to_allocate);

    return (malloced_base_address == 0x0 ? 0x0 : malloced_base_address);
}


// free: deallocates memory provided the base address of the segment tobe free'd.
void free(struct heap* _heap, void* _base){

    free_blocks(_heap, ((uint32_t)_base - (uint32_t) _heap->heap_base)/KERNEL_HB_SIZE);

}


void* malloc_blocks(struct heap* _heap, uint32_t blocks_to_allocate){

    // we need to find a set of blocks which are free in the heap entry table
    uint32_t start_entry = find_free_entry(_heap, blocks_to_allocate);
    if(start_entry == -ERR_MALLOC_FAIL){
        return 0x0;
    }

    // once we have allocated those blocks, we mark them taken.
    mark_blocks_taken(_heap,start_entry, start_entry + blocks_to_allocate -1);


    return (void*)(_heap->heap_base + start_entry*KERNEL_HB_SIZE);

}

// deallocate memory starting at some block '_block'
void free_blocks(struct heap* _heap, uint32_t start_block){


        for(uint32_t block = start_block; block < _heap->entry_table->num_entries; block++){
            if(!(_heap->entry_table->entries[block] & HEAP_BLOCK_HAS_NEXT)) break;

            _heap->entry_table->entries[block] = HEAP_BLOCK_ENTRY_FREE;

        }


}



// we can only allocate blocks. so we need to round up the requested size until its divisible by the block size.
static uint32_t get_aligned_heap_size(size_t _size){
    if(_size % KERNEL_HB_SIZE == 0) return _size;

    return _size - (_size % KERNEL_HB_SIZE) + KERNEL_HB_SIZE;

}

// check if heap start address matches with one of the block's start address
static uint8_t validate_heap_alignment(void* addr){

    return ((uint32_t) addr % KERNEL_HB_SIZE) == 0;
}



static int validate_heap_table(void* saddr, void* eaddr, struct hb_table * _hb_table){

    return _hb_table->num_entries == (eaddr - saddr)/KERNEL_HB_SIZE;

}



uint32_t find_free_entry(struct heap* _heap, uint32_t blocks_to_allocate){
    uint32_t block_count = 0;
    uint32_t block_start = -1;

    for(uint32_t current_block = 0; current_block < _heap->entry_table->num_entries; current_block++){

        if(_heap->entry_table->entries[current_block] != HEAP_BLOCK_ENTRY_FREE){
            block_start = -1;
            block_count = 0;
            continue;
        }

        if(block_start == -1){ // if this is the first block we found
            block_start = current_block;
        }

        block_count++;

        if(block_count == blocks_to_allocate){
            return block_start;
        }
    }

    // if we didnt find a suitable fit for 'blocks_to_allocate', we return an error.
    return -ERR_MALLOC_FAIL;

}


// once we have allocated some number of blocks, we need to mark them in the heap table, so that we dont reuse them until free'd.
void mark_blocks_taken(struct heap* _heap, uint32_t sblock, uint32_t eblock){ // passing in starting block and ending block

    if(sblock == eblock){ // only one block given
        _heap->entry_table->entries[sblock] = HEAP_BLOCK_ENTRY_TAKEN | HEAP_BLOCK_IS_FIRST;
        return;
    }

    for(uint32_t block = sblock; block <= eblock; block++){
        if(block == sblock){
            _heap->entry_table->entries[block] = HEAP_BLOCK_ENTRY_TAKEN | HEAP_BLOCK_IS_FIRST | HEAP_BLOCK_HAS_NEXT;
            continue;
        }

        if(block == eblock){
            _heap->entry_table->entries[block] = HEAP_BLOCK_ENTRY_TAKEN;
            continue;
        }

        _heap->entry_table->entries[block] = HEAP_BLOCK_ENTRY_TAKEN | HEAP_BLOCK_HAS_NEXT;
    }

    return;

}

