#include "fat16.h"
#include "../vfs/vfs.h"
#include "../../string/string.h"
#include "../../drivers/disk/disk.h"
#include "../../kernel/kstatus.h"
#include "../../mm/memory.h"
#include "../../mm/heap/kheap.h"

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
int fat16_close(void* fd_private_data);
int fat16_fstat(void* fd_private_data, struct file_stat* stat);

struct filesystem fat16 = {
    .resolve = fat16_resolve,
    .open    = fat16_open,
    .close   = fat16_close,
    .fstat   = fat16_fstat
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
    uint32_t            first_sector;
    uint32_t            last_sector;
};

struct fat16_unit{
    union{ // could either be a directory or file
        struct fat16_directory* directory;
        struct fat16_file*      file;
    };

    uint8_t fat16_unit_type; // directory/file
};

struct fat16_file_descriptor{ 
    struct fat16_unit* unit; // can be a directory or a file.
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

// returns number of files in given directory.
uint32_t fat16_total_files_in_directory(struct disk* _disk, struct fat16_private_data* fat16_private, struct fat16_directory* dir){

    // point streamer to the start of directory.
    disk_stream_seek(fat16_private->directory_streamer, dir->first_sector * _disk->sector_size);

    uint32_t total_files = 0;

    struct fat16_file temp_file;
    memset(&temp_file, 0, sizeof(temp_file));

    do{
        disk_stream_read(fat16_private->directory_streamer, &temp_file, sizeof(temp_file));
        total_files++;
    }while(temp_file.name[0] == 0xE5);


    return total_files - 1;

}

int8_t fat16_get_root(struct disk* _disk, struct fat16_private_data* fat16_private, struct fat16_directory* root){

    uint32_t fat16_root_sector           = (fat16_private->header.bpb.fat_copies * fat16_private->header.bpb.sectors_per_fat) + fat16_private->header.bpb.reserved_sectors;
    uint32_t fat16_root_entries          = fat16_private->header.bpb.root_dir_entries;
    uint32_t fat16_root_size             = fat16_root_entries * sizeof(struct fat16_file);
    uint32_t fat16_root_sectors_occupied = fat16_root_size / _disk->sector_size + (fat16_root_size % _disk->sector_size != 0);

    // first file of the root directory
    root->files = (struct fat16_file*) kzalloc(fat16_root_size);
    struct disk_stream* root_streamer = disk_stream_new();

    if(root->files == 0x0){
        // couldn't create memory for root directory.
        return -ERR_MALLOC_FAIL;
    }

    // read the root directory into our structure.
    disk_stream_seek(root_streamer, fat16_root_sector * _disk->sector_size);
    disk_stream_read(root_streamer, root->files, fat16_root_size);

    root->first_sector = fat16_root_sector;
    root->last_sector  = root->first_sector + fat16_root_sectors_occupied - 1;
    root->total_files  = fat16_total_files_in_directory(_disk, fat16_private, root); 

    disk_stream_close(root_streamer);


    return SUCCESS;

}

int8_t fat16_resolve(struct disk* _disk){

    struct fat16_private_data* fat16_private = (struct fat16_private_data*) kzalloc(sizeof(struct fat16_private_data));


    // initialising private data structure
    fat16_private->cluster_streamer     = disk_stream_new();
    fat16_private->directory_streamer   = disk_stream_new();
    fat16_private->fat_streamer         = disk_stream_new();

    // general purpose streamer 
    struct disk_stream* fat16_streamer  = disk_stream_new();

    // reading the fs header in the disk into private data structure.
    disk_stream_read(fat16_streamer, &fat16_private->header, sizeof(fat16_private->header));

    if(fat16_private->header.extended.ebpb.signature != 0x29){

        disk_stream_close(fat16_streamer);
        kfree(fat16_private);
        
        _disk->fs_private_data = 0;

        return -ERR_FS_INV_SIGN; // invalid signature
    }

    // link filesystem private data to the disk
    _disk->fs_private_data = fat16_private;

    if(fat16_get_root(_disk, fat16_private, &fat16_private->root) != SUCCESS){

        disk_stream_close(fat16_streamer);
        kfree(fat16_private);

        _disk->fs_private_data = 0;

        return -ERR_FS_ROOT_NOT_FOUND; // couldn't locate root directory on disk
    }


    disk_stream_close(fat16_streamer);


    return SUCCESS; // filesystem resolved


}

void* fat16_open(struct disk* _disk, struct path* _path, uint8_t mode){
    if(mode != READ){
            return 0;
    }
    return 0;
}

int fat16_close(void* fd_private_data){

    return 0;
}

int fat16_fstat(void* fd_private_data, struct file_stat* stat){
    
    struct fat16_file_descriptor* _fd = (struct fat16_file_descriptor*) (fd_private_data);
    struct fat16_unit*            _fu = _fd->unit;

    if(_fu->fat16_unit_type == FAT16_DIRECTORY){
        return -ERR_INVARG; // cannot stat a directory
    }

    struct fat16_file* _file = _fu->file;

    stat->size = _file->size;
    stat->flags |= _file->attributes;

    return SUCCESS;

}