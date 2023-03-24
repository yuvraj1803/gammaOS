#ifndef __DISK_H__
#define __DISK_H__

#include <stdint.h>
#include "../../config.h"

#define PRIMARY_DISK        'A'

struct disk_stream{
    uint32_t streamer_pos;
};

struct disk{
    char     disk_id;
};

struct disk* disk_init();
struct disk* disk_get(char disk_id);

struct disk_stream* disk_stream_init(struct disk_stream* streamer, uint32_t position);
void                disk_stream_seek(struct disk_stream* streamer, uint32_t position);
void                disk_stream_close(struct disk_stream* streamer);
void                disk_stream_read(struct disk_stream* streamer, void* read_buffer, uint32_t bytes_to_read);

void ata_lba_read_sector(uint32_t lba, uint32_t sector_count, void* read_buffer);


#endif