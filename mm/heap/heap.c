#include "heap.h"


static uint32_t get_aligned_heap_size(size_t _size){
    if(_size % KERNEL_HB_SIZE == 0) return _size;

    return _size - (_size % KERNEL_HB_SIZE) + KERNEL_HB_SIZE;

}

int32_t create_heap(struct heap* _heap, void* saddr, void* eaddr, struct hb_table* _){
    // return val < 0 denotes failure.

    return 0;
}

void* malloc(struct heap* heap, size_t _size){
    size_t aligned_size = get_aligned_heap_size(_size);
    uint32_t blocks_to_allocate = aligned_size / KERNEL_HB_SIZE;

    if(blocks_to_allocate){}

    return 0;
}

void free(struct heap* heap, void* _base){

}