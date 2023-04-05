#include "disk.h"
#include "../../io/io.h"
#include "../../kernel/kernel.h"
#include "../../mm/heap/kheap.h"
#include "../../fs/vfs/vfs.h"

#define READ_SECTORS_WITH_RETRY             0x20
#define SECTOR_BUFFER_REQUIRES_SERVICING    0b00001000
#define SECTOR_SIZE_IN_BYTES                512

struct disk disks[MAX_NUMBER_OF_DISKS];

// initialise memory for MAX_NUMBER_OF_DISKS and add primary disk. Default disk is A
struct disk* disk_init(char disk_id){

    if(disk_id < 'A' || disk_id >= 'A' + MAX_NUMBER_OF_DISKS){
        return 0; // illegal disk begin initialised.
    }

    disks[disk_id-'A'].disk_id = disk_id;
    disks[disk_id-'A'].sector_size  = 512;

    disks[disk_id-'A'].fs = vfs_resolve(&disks[disk_id-'A']);

    return &disks[disk_id-'A'];
}

struct disk* disk_get(char disk_id){
    return &disks[disk_id-'A'];
}

struct disk_stream* disk_stream_new(){
    struct disk_stream* streamer = (struct disk_stream*) kzalloc(sizeof(struct disk_stream));
    streamer->streamer_pos = 0;

    return streamer;
}

// returns a disk streamer pointing at given position
struct disk_stream* disk_stream_init(struct disk_stream* streamer, uint32_t position){
    streamer->streamer_pos = position;

    return streamer;
}

// given a disk streamer, points it to given location.
void disk_stream_seek(struct disk_stream* streamer, uint32_t position){
    streamer->streamer_pos = position;
}

void disk_stream_close(struct disk_stream* streamer){
    kfree(streamer);
}

void disk_stream_read(struct disk_stream* streamer, void* read_buffer, uint32_t bytes_to_read){


    // finding which sector the streamer is currently at
    uint32_t sector        = streamer->streamer_pos / SECTOR_SIZE_IN_BYTES;
    uint32_t sector_offset = streamer->streamer_pos % SECTOR_SIZE_IN_BYTES;

    // if we are trying to read out of bounds of current sector
    uint8_t is_overflowing = (sector_offset + bytes_to_read) >= SECTOR_SIZE_IN_BYTES;

    uint8_t temp_buffer[SECTOR_SIZE_IN_BYTES];
    uint32_t temp_bytes_to_read = bytes_to_read;


    // we can only read one sector at a time, so we first read the current sector in which the streamer is at.
    if(is_overflowing){
        bytes_to_read -= (sector_offset + bytes_to_read) - SECTOR_SIZE_IN_BYTES;
    }

    // reading the current sector and adding it to the read buffer provided
    ata_lba_read_sector(sector, 1, temp_buffer);

    for(int index = 0;index < bytes_to_read; index++){
        *(uint8_t*) read_buffer++ = temp_buffer[sector_offset + index];
    }

    streamer->streamer_pos += bytes_to_read;

    // now if we tried to read out of bounds, we recursively read the next sector.
    if(is_overflowing){
        disk_stream_read(streamer, read_buffer, temp_bytes_to_read - bytes_to_read);
    }


}

void ata_lba_read_sector(uint32_t lba, uint32_t sector_count, void* read_buffer){


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