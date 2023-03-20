#ifndef __PAGING_H__
#define __PAGING_H__

#include <stdint.h>

#define PAGE_PRESENT        0b00000001  /*If page is actually at physical memory at that time*/
#define PAGE_WRITE_ACCESS   0b00000010  /*If page can be written to*/
#define PAGE_USER_ACCESS    0b00000100  /*If page access is for everybody.*/
#define PAGE_WRITE_THROUGH  0b00001000  /*Write through caching employed for page*/
#define PAGE_CACHE_DISABLE  0b00010000  /*Page wont be cached*/

#define PAGE_DIR_ENTRIES    1024
#define PAGE_TABLE_ENTRIES  1024
#define PAGE_SIZE           4096



struct page_directory{

    uint32_t* page_directory_entries; 

};

struct vaddr_space{
    struct page_directory* pd;
};

void enable_paging();

void load_page_directory(struct page_directory* pd);
struct vaddr_space* create_virtual_address_space(uint8_t vaddr_space_flags);

int8_t map_vaddr_to_val(struct page_directory* pd, void* vaddr, uint32_t val);

#endif