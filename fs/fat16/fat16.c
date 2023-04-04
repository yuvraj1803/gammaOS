#include "fat16.h"
#include "../vfs/vfs.h"
#include "../../string/string.h"
#include "../../drivers/disk/disk.h"

#define FAT16_SIGNATURE     0x29
#define FAT16_ENTRY_SIZE    0x02
#define FAT16_BAD_SECTOR    0xFF7
#define FAT16_UNUSED        0x00       

#define FAT16_DIRECTORY 0
#define FAT16_FILE 1

#define FAT16_FILE_READ_ONLY 0x01
#define FAT16_FILE_HIDDEN 0x02
#define FAT16_FILE_SYSTEM 0x04
#define FAT16_FILE_VOLUME_LABEL 0x08
#define FAT16_FILE_SUBDIRECTORY 0x10
#define FAT16_FILE_ARCHIVED 0x20
#define FAT16_FILE_DEVICE 0x40
#define FAT16_FILE_RESERVED 0x80

int8_t fat16_resolve(struct disk* _disk);
void* fat16_open(struct disk* _disk, struct path* _path, uint8_t mode);

struct filesystem fat16 = {
    .resolve = fat16_resolve,
    .open    = fat16_open
};

struct fat16_bpb{ // bios parameter block

    uint8_t     short_jump_instruction_and_nop[3];
    uint8_t     oem_identifier[8];
    uint16_t    bytes_per_sector;
    uint8_t     sectors_per_cluster;
    uint16_t    reserved_sectors;
    uint8_t     fat_copies;
    uint16_t    root_dir_entries;
    uint16_t    number_of_sectors;
    uint8_t     media_type;
    uint16_t    sectors_per_fat;
    uint16_t    sectors_per_track;
    uint16_t    number_of_heads;
    uint32_t    hidden_setors;
    uint32_t    sectors_big;

} __attribute__((packed));

struct fat16_ebpb{ // extended bios parameter block
    uint8_t     drive_number;
    uint8_t     win_nt_bit;
    uint8_t     signature;
    uint32_t    volume_id;
    uint8_t     volume_id_str[11];
    uint8_t     system_id_str[8];
}__attribute__((packed));


struct fat16_header{

    struct fat16_bpb bpb;            // bios parameter block
    union {struct fat16_ebpb ebpb;} extended; // extended bios parameter block

};


struct fat16_file{
    uint8_t     name[8];
    uint8_t     extention[3];
    uint8_t     attributes;
    uint8_t     reserved;
    uint8_t     creation_time_in_tenths_of_a_sec;
    uint16_t    creation_time;
    uint16_t    creation_date;
    uint16_t    last_access;
    uint16_t    high_16_bits_of_first_cluster;
    uint16_t    last_modification_time;
    uint16_t    last_modification_date;
    uint16_t    low_16_bits_of_first_cluster;
    uint32_t    size;
}__attribute__((packed));

struct fat16_directory{
    struct fat16_file*  files;
    uint32_t            total_files;
    uint32_t            first_cluster;
    uint32_t            last_cluster;
};

struct fat16_unit{
    union{ // could either be a directory or file
        struct fat16_directory* directory;
        struct fat16_file*      file;
    };

    uint8_t fat16_unit_type; // directory/file
};

struct fat16_file_descriptor{
    struct fat16_file* file;
    uint32_t           file_position; // position inside the file at given time
};

struct fat16_private_data{
    struct fat16_header         header;
    struct fat16_directory      root; // root directory 

    struct disk_stream*         cluster_streamer;
    struct disk_stream*         fat_streamer;
    struct disk_stream*         directory_streamer;
};

struct filesystem* fat16_init(){

    strcpy(fat16.name, "fat16");    

    return &fat16;

}

int8_t fat16_resolve(struct disk* _disk){
    return 0;
}

void* fat16_open(struct disk* _disk, struct path* _path, uint8_t mode){
    return 0;
}
