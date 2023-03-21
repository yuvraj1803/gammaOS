#ifndef __DISK_H__
#define __DISK_H__

#include <stdint.h>

typedef uint32_t disk_type;



struct disk{

    


};



void ata_lba_read_sector(uint32_t lba, uint32_t sector_count, void* read_buffer);

#endif