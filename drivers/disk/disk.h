#ifndef __DISK_H__
#define __DISK_H__

#include <stdint.h>

typedef uint32_t disk_type;



struct disk{

    uint16_t    disk_index;
    uint16_t    sector_size;
    disk_type   type;

};



void ata_lba_read_sector(uint32_t lba, uint32_t sector_count, void* read_buffer);

#endif