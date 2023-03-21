#include "disk.h"
#include "../../io/io.h"
#include "../../kernel/kernel.h"

#define READ_SECTORS_WITH_RETRY             0x20
#define SECTOR_BUFFER_REQUIRES_SERVICING    0b00001000
#define SECTOR_SIZE_IN_BYTES                512

void ata_lba_read_sector(int lba, int sector_count, void* read_buffer){

    outb(0x1F6, (lba >> 24) | 0xE0);
    outb(0x1F2, sector_count);
    outb(0x1F3, (uint16_t) (lba & 0xff));
    outb(0x1F4, (uint16_t) (lba >> 8));
    outb(0x1F5, (uint16_t) (lba >> 16));
    outb(0x1F7, READ_SECTORS_WITH_RETRY);

    uint16_t* read_ptr = (uint16_t*) read_buffer;

    for(int sector = 0; sector < sector_count; sector++){
        
        // wait until the data is read
        while(!(insb(0x1F7) & SECTOR_BUFFER_REQUIRES_SERVICING));


        // reading 2 bytes at a time
        for(int data=0;data < SECTOR_SIZE_IN_BYTES / 2; data++){
            *read_ptr = insw(0x1F0);
            read_ptr++;
        }


    }

}