/*
 *
 * Copyright (c) 2023 Yuvraj Sakshith <ysakshith@gmail.com>
 *
 */

#ifndef __DISK_H__
#define __DISK_H__

#include <stdint.h>
#include "../../config.h"
#include "../../fs/vfs/vfs.h"


struct disk_stream{
    uint32_t streamer_pos;
};

struct disk{
    char     disk_id;
    uint32_t sector_size;
    struct filesystem* fs;
    void* fs_private_data;
};


struct disk* disk_init(char disk_id);
struct disk* disk_get(char disk_id);

struct disk_stream* disk_stream_init(struct disk_stream* streamer, uint32_t position);
void                disk_stream_seek(struct disk_stream* streamer, uint32_t position);
void                disk_stream_close(struct disk_stream* streamer);
void                disk_stream_read(struct disk_stream* streamer, void* read_buffer, uint32_t bytes_to_read);
struct disk_stream* disk_stream_new();


void ata_lba_read_sector(uint32_t lba, uint32_t sector_count, void* read_buffer);


#endif