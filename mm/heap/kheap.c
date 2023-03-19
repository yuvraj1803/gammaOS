#include "kheap.h"
#include "heap.h"
#include "../../config.h"
#include "../../kernel/kernel.h"

#include <stdint.h>

struct heap     kheap;       // kernel heap
struct hb_table kheap_table; // kernel heap table

void kheap_init(){

    uint32_t kheap_entry_count = KERNEL_HEAP_SIZE / KERNEL_HB_SIZE; /* Number of entries in kernel heap table*/


    kheap_table.entries = (hb_entry*) KERNEL_HEAP_TABLE_BASE; /* Base address of kernel heap table*/
    kheap_table.num_entries = kheap_entry_count;

    void* kheap_end = (void*) (KERNEL_HEAP_BASE + KERNEL_HEAP_SIZE); /* End of kernel heap*/

    int32_t kheap_create_status = create_heap(&kheap, (void*) KERNEL_HEAP_BASE, (void*) kheap_end, &kheap_table);

    if(kheap_create_status < 0){
        kpanic("[KERNEL ERROR]: kheap initialisation failure\n");
    }


}

void* kmalloc(uint32_t _size_in_bytes){
    return malloc(&kheap, _size_in_bytes);
}

void kfree(void* _base){
    free(&kheap, _base);
}