/*
 *
 * Copyright (c) 2023 Yuvraj Sakshith <ysakshith@gmail.com>
 *
 */

#ifndef __PROCESS_H__
#define __PROCESS_H__

#include "../../../config.h"
#include <stdint.h>

struct process* process_get(uint16_t pid);
struct process* process_new(const char* filename);  

enum{
    PROCESS_ELF_FILE,
    PROCESS_BIN_FILE
};

struct process{

    uint16_t pid; // process id

    char file[FS_MAX_PATH_LENGTH];

    struct task* task; // main task

    void* mem_allocations[PROCESS_MAX_PROCESS_MEM_ALLOCATIONS]; // all dynamic memory allocations done by the process

    void* physical_address; // pointer to process memory
    void* stack; // pointer to process stack

    uint32_t size; // size of memory pointed by "physical_address"

    uint8_t file_type;

};

#endif