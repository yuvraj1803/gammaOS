#ifndef __DISK_H__
#define __DISK_H__

#include <stdint.h>

struct disk{

    


};



void ata_lba_read_sector(int lba, int sector_count, void* read_buffer);

#endif