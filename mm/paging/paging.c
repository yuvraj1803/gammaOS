/*
 *
 * Copyright (c) 2023 Yuvraj Sakshith <ysakshith@gmail.com>
 *
 */

#include "paging.h"
#include "../heap/kheap.h"
#include "../../kernel/kstatus.h"

/* Pointer to the page_directory currently being used*/
struct page_directory *current_page_directory;

void load_pd_to_cr3(uint32_t *pd_base); /*Loads a new page directory into cr3 register*/
void load_page_directory(struct page_directory *pd);
int8_t get_page_indices(void *vaddr, uint32_t *pd_entry, uint32_t *pt_entry);
int8_t map_vaddr_to_val(struct page_directory *pd, void *vaddr, uint32_t val);
struct page_directory *new_page_directory(uint8_t page_entry_flags);
uint8_t address_page_aligned(void *vaddr);

/* This function loads a new page directory into cr3 register.*/
/* Can also be used to switch between directories*/
void load_page_directory(struct page_directory *pd)
{
    load_pd_to_cr3(pd->page_directory_entries);
    current_page_directory = pd;
}

uint8_t address_page_aligned(void *vaddr)
{
    return (uint32_t)vaddr % PAGE_SIZE == 0;
}

uint32_t paging_align_to_page(uint32_t address){
    if(address %PAGE_SIZE){
        return address - address%PAGE_SIZE + PAGE_SIZE;
    }

    return address; // already aligned.
}


int8_t paging_map_range(struct vaddr_space* _vspace, void* vaddr, void* paddr, void* endaddr, int flags){

    if(!address_page_aligned(vaddr) || !address_page_aligned(paddr) || !address_page_aligned(endaddr)){
        return -ERR_PROCESS_MAPPING_FAILED;
    }

    if((uint32_t) endaddr < (uint32_t) paddr) return -ERR_PROCESS_MAPPING_FAILED;

    uint32_t total_pages = ((uint32_t)endaddr - (uint32_t)paddr)/PAGE_SIZE;

    for(uint32_t page=0;page < total_pages;page++){
        map_vaddr_to_val(_vspace->pd, vaddr, (uint32_t) paddr | flags);

        vaddr += PAGE_SIZE;
        paddr += PAGE_SIZE;
    }


    return SUCCESS;

}



int8_t get_page_indices(void *vaddr, uint32_t *pd_entry, uint32_t *pt_entry)
{
    if (!address_page_aligned(vaddr))
    { /*Virtual address provided has not match base address of some page.*/
        return -ERR_INVARG;
    }

    *pd_entry = (uint32_t)vaddr / (PAGE_SIZE * PAGE_TABLE_ENTRIES);
    *pt_entry = ((uint32_t)vaddr % (PAGE_TABLE_ENTRIES * PAGE_SIZE) / PAGE_SIZE);

    return SUCCESS;
}

/*Maps a virtual address to a given value.*/
int8_t map_vaddr_to_val(struct page_directory *pd, void *vaddr, uint32_t val)
{

    if (!address_page_aligned(vaddr))
    {
        return -ERR_INVARG;
    }

    uint32_t pd_index; // page directory index
    uint32_t pt_index; // page table index    ... of the virtual address provided

    get_page_indices(vaddr, &pd_index, &pt_index);

    uint32_t *pt = (uint32_t *)(pd->page_directory_entries[pd_index] & (0xfffff000));
    pt[pt_index] = val;

    return SUCCESS;
}

/* Creates a new page directory with given flags. */
/* Same flags are also placed in the page table entries*/
struct page_directory *new_page_directory(uint8_t page_entry_flags)
{

    uint32_t *_page_directory_entries = kzalloc(sizeof(uint32_t) * PAGE_DIR_ENTRIES);
    uint32_t pd_entry_offset = 0;

    for (uint16_t pd_entry = 0; pd_entry < PAGE_DIR_ENTRIES; pd_entry++)
    {

        uint32_t *_page_table_entries = kzalloc(sizeof(uint32_t) * PAGE_TABLE_ENTRIES);

        for (uint16_t pt_entry = 0; pt_entry < PAGE_TABLE_ENTRIES; pt_entry++)
        {
            _page_table_entries[pt_entry] = (pd_entry_offset + pt_entry * PAGE_SIZE) | page_entry_flags;
        }
        /* increase the offset by the number of entries in the page table *  page size.
         this is going to be the base address of the first page in the next page
         table entry
         */
        pd_entry_offset += PAGE_TABLE_ENTRIES * PAGE_SIZE;

        _page_directory_entries[pd_entry] = (uint32_t)_page_table_entries | page_entry_flags | PAGE_WRITE_ACCESS;
    }

    struct page_directory *_page_directory_to_return = kzalloc(sizeof(struct page_directory));
    _page_directory_to_return->page_directory_entries = _page_directory_entries;

    return _page_directory_to_return;
}

struct vaddr_space *create_virtual_address_space(uint8_t vaddr_space_flags)
{

    struct vaddr_space *_vspace = (struct vaddr_space *)kzalloc(sizeof(struct vaddr_space));
    _vspace->pd = new_page_directory(vaddr_space_flags);

    return _vspace;
}

void destroy_virtual_address_space(struct vaddr_space* _space){

    // free every page table one by one.
    for(int pd_entry=0;pd_entry < PAGE_DIR_ENTRIES;pd_entry++){
        uint32_t entry = _space->pd->page_directory_entries[pd_entry];
        uint32_t* pt = (uint32_t*) (entry & 0xfffff000);
        kfree(pt);
    }

    kfree(_space->pd); // free page directory
    kfree(_space);

}

uint32_t paging_virt_to_phy(struct vaddr_space* _vspace, void* vaddr){
    uint32_t pd_index;
    uint32_t pt_index;

    get_page_indices(vaddr,&pd_index,&pt_index);

    uint32_t pt_entry = _vspace->pd->page_directory_entries[pd_index];
    uint32_t* pt = (uint32_t*) (pt_entry & 0xfffff000);

    return pt[pt_index];

}