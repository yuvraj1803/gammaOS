#include "paging.h"
#include "../heap/kheap.h"
#include "../../kernel/kstatus.h"


/* Pointer to the page_directory currently being used*/
struct page_directory* current_page_directory;

void load_pd_to_cr3(uint32_t* pd_base); /*Loads a new page directory into cr3 register*/
void load_page_directory(struct page_directory* pd);
int8_t get_page_indices(void* vaddr, uint32_t* pd_entry, uint32_t* pt_entry);
int8_t map_vaddr_to_val(struct page_directory* pd, void* vaddr, uint32_t val);
struct page_directory* new_page_directory(uint8_t page_entry_flags);
uint8_t address_page_aligned(void* vaddr);


/* This function loads a new page directory into cr3 register.*/
/* Can also be used to switch between directories*/
void load_page_directory(struct page_directory* pd){
    load_pd_to_cr3(pd->page_directory_entries);
    current_page_directory = pd;
}

uint8_t address_page_aligned(void* vaddr){
    return (uint32_t) vaddr % PAGE_SIZE == 0;
}

int8_t get_page_indices(void* vaddr, uint32_t* pd_entry, uint32_t* pt_entry){
    if(!address_page_aligned(vaddr)){ /*Virtual address provided has not match base address of some page.*/
        return -ERR_INVARG;
    }

    *pd_entry = (uint32_t) vaddr / (PAGE_SIZE * PAGE_TABLE_ENTRIES);
    *pt_entry = ((uint32_t) vaddr % (PAGE_TABLE_ENTRIES * PAGE_SIZE) / PAGE_SIZE);

    return SUCCESS;
}

/*Maps a virtual address to a given value.*/
int8_t map_vaddr_to_val(struct page_directory* pd, void* vaddr, uint32_t val){

    if(!address_page_aligned(vaddr)){
        return -ERR_INVARG;
    }

    uint32_t pd_index; // page directory index
    uint32_t pt_index; // page table index    ... of the virtual address provided

    get_page_indices(vaddr, &pd_index, &pt_index);

    uint32_t* pt = (uint32_t*) (pd->page_directory_entries[pd_index] & (0xfffff000));
    pt[pt_index] = val;

    return SUCCESS;

}

/* Creates a new page directory with given flags. */
/* Same flags are also placed in the page table entries*/
struct page_directory* new_page_directory(uint8_t page_entry_flags){

    uint32_t * _page_directory_entries = kzalloc(sizeof(uint32_t) * PAGE_DIR_ENTRIES);
    uint32_t pd_entry_offset = 0;

    for(uint16_t pd_entry=0; pd_entry < PAGE_DIR_ENTRIES; pd_entry++){
        
        uint32_t* _page_table_entries = kzalloc(sizeof(uint32_t) * PAGE_TABLE_ENTRIES);

        for(uint16_t pt_entry=0; pt_entry < PAGE_TABLE_ENTRIES; pt_entry++){
            _page_table_entries[pt_entry] = (pd_entry_offset + pt_entry * PAGE_SIZE) | page_entry_flags;
        }
        /* increase the offset by the number of entries in the page table *  page size.
         this is going to be the base address of the first page in the next page
         table entry
         */
        pd_entry_offset += PAGE_TABLE_ENTRIES * PAGE_SIZE;

        _page_directory_entries[pd_entry] = (uint32_t) _page_table_entries | page_entry_flags | PAGE_WRITE_ACCESS;
    }

    struct page_directory* _page_directory_to_return = kzalloc(sizeof(struct page_directory));
    _page_directory_to_return->page_directory_entries = _page_directory_entries;

    return _page_directory_to_return;

}

struct vaddr_space* create_virtual_address_space(uint8_t vaddr_space_flags){

    struct vaddr_space* _vspace = (struct vaddr_space*) kzalloc(sizeof(struct vaddr_space));
    _vspace->pd = new_page_directory(vaddr_space_flags);

    return _vspace;

}