#include "kheap.h"
#include "heap.h"
#include "../../config.h"
#include "../../kernel/kernel.h"

#include <stdint.h>

struct heap     kheap;
struct hb_table kheap_table;


void kheap_init(){

    uint32_t kheap_entry_count = KERNEL_HEAP_SIZE / KERNEL_HB_SIZE; /* Number of entries in kernel heap table*/

    kheap_table.entries = (hb_entry*) KERNEL_HEAP_TABLE_BASE;
    kheap_table.num_entries = kheap_entry_count;

    void* kheap_end = (void*) (KERNEL_HEAP_BASE + KERNEL_HEAP_SIZE);

    int32_t kheap_create_status = create_heap(&kheap, (void*) KERNEL_HEAP_BASE, kheap_end, &kheap_table);
    
    if(kheap_create_status < 0){
        kpanic("[KERNEL ERROR]: kheap initialisation failure");
    }

}