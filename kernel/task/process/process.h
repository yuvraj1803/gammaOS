/*
 *
 * Copyright (c) 2023 Yuvraj Sakshith <ysakshith@gmail.com>
 *
 */

#ifndef __PROCESS_H__
#define __PROCESS_H__

#include "../../../config.h"
#include <stdint.h>
#include "../../../g-loader/elf/elf.h"

struct process *process_get(uint16_t pid);
struct process *process_new(const char *filename);
struct process *process_current();
void* process_malloc(uint32_t size);
void* process_free(void* base_addr);
int process_kill(struct process* _process);

void process_init();


enum
{
    PROCESS_ELF_FILE,
    PROCESS_BIN_FILE
};

struct process
{

    uint16_t pid; // process id

    char file[FS_MAX_PATH_LENGTH];

    struct task *task; // main task

    void *mem_allocations[PROCESS_MAX_PROCESS_MEM_ALLOCATIONS]; // all dynamic memory allocations done by the process
    uint32_t mem_allocations_size[PROCESS_MAX_PROCESS_MEM_ALLOCATIONS]; // size of the mem_allocations[i]

    union{
        void*       raw_data;
        ELF_FILE*   elf_data;
    };
    
    void *stack;            // pointer to process stack

    uint32_t size; // size of memory pointed by "physical_address"

    uint8_t file_type;

    // keyboard buffer for the process

    struct keyboard_buffer
    {
        char buffer[PROCESS_MAX_KEYBOARD_BUFFER_SIZE];
        int head;
        int tail;
    } keyboard_buf;
};

#endif