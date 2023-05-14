/*
 *
 * Copyright (c) 2023 Yuvraj Sakshith <ysakshith@gmail.com>
 *
 */

#include "process.h"
#include "../../../config.h"
#include "../../../kernel/kstatus.h"
#include "../../../string/string.h"
#include "../../../fs/file.h"
#include "../../../mm/memory.h"
#include "../task.h"
#include "../../../mm/heap/kheap.h"
#include "../../../mm/paging/paging.h"
#include "../../../g-loader/g-loader.h"
#include "../../../g-loader/elf/elf.h"

// #include "../../../kernel/kernel.h"

struct process* current_process = 0;
static struct process* process_list[PROCESS_MAX_PROCESSES];

uint8_t processes_initialised = 0;


struct process* process_current(){
    return current_process;
}

static void process_init(){
    memset(process_list, 0, sizeof(process_list));
}

struct process* process_get(uint16_t pid){
    
    if(pid < 0 || pid >= PROCESS_MAX_PROCESSES){
        return 0;
    }

    return process_list[pid];
}

static uint16_t process_get_free_process_id(){

    uint16_t pid = -ERR_MAX_PROCESS_LIMIT_REACHED;

    for(int _pid = 0;_pid < PROCESS_MAX_PROCESSES;_pid++){
        if(process_list[_pid] == 0){
            pid = _pid;
            break;
        }
    }

    return pid;

}

static int8_t process_load_binary(struct process* _process){
    char* filename = _process->file;

    int fd = fopen(filename, 'r');

    if(fd < 0){
        return -ERR_PROCESS_LOAD_FAIL;
    }

    struct file_stat stat;
    if(fstat(fd, &stat) == -ERR_FD_NOT_FOUND){
        fclose(fd);
        return -ERR_PROCESS_LOAD_FAIL;
    }

    _process->size = stat.size;
    _process->raw_data = kzalloc(_process->size);

    if(!_process->raw_data){
        fclose(fd);
        return -ERR_PROCESS_LOAD_FAIL;
    }

    if(fread(_process->raw_data, _process->size, 1, fd) < 0){
        kfree(_process->raw_data);
        fclose(fd);
        return -ERR_PROCESS_LOAD_FAIL;
    }

    _process->file_type = PROCESS_BIN_FILE;

    fclose(fd);

    return SUCCESS;

}

static int8_t process_load_elf(struct process* _process){
    ELF_FILE* elf_file;
    char* filename = _process->file;

    if(gloader_load_elf(filename, &elf_file) < 0) return -ERR_PROCESS_LOAD_FAIL;


    _process->file_type = PROCESS_ELF_FILE;
    _process->elf_data = elf_file;


    return SUCCESS;

}

static int8_t process_load_data(struct process* _process){


    int elf_load_status = process_load_elf(_process);

    // we try to check if the file provided is an elf file first.
    // if it is, then we load it and leave.
    // otherwise, we treat it as a raw binary and load it the way it is.

    if(elf_load_status < 0){
        if(process_load_binary(_process) < 0) return -ERR_PROCESS_LOAD_FAIL;
    }

    return SUCCESS;
}

static int8_t process_map_binary(struct process* _process){

    if(paging_map_range(_process->task->task_space, (void*)TASK_DEFAULT_START, _process->raw_data, (void*)  paging_align_to_page_upper((uint32_t)_process->raw_data + _process->size), PAGE_WRITE_ACCESS | PAGE_PRESENT | PAGE_USER_ACCESS) < 0){
        return -ERR_PROCESS_MAPPING_FAILED;
    }

    return SUCCESS;

}

static int8_t process_map_elf(struct process* _process){
    
    ELF_FILE*   elf_file = _process->elf_data;
    Elf32_Ehdr* ehdr = ELF_get_Ehdr(elf_file);

    for(int nphdr = 0; nphdr < ehdr->e_phnum; nphdr++){
        Elf32_Phdr* phdr = ELF_get_Phdr(elf_file, nphdr); // get the 'nphdr'th program header

        // physical address of current program header segment 
        void* elf_file_code_physical_address = (void*)((uint32_t) elf_file->data + phdr->p_offset);
        int page_flags = PAGE_PRESENT | PAGE_USER_ACCESS; // write access not given

        if(phdr->p_flags & PF_W){
            // write access is given only if certain flag set in program header.
            page_flags |= PAGE_WRITE_ACCESS;
        }

        if(paging_map_range(_process->task->task_space, (void*)paging_align_to_page_lower(phdr->p_vaddr), (void*) paging_align_to_page_lower((uint32_t)elf_file_code_physical_address), (void*) paging_align_to_page_upper((uint32_t) elf_file_code_physical_address + phdr->p_memsz) ,page_flags) < 0){
            return -ERR_PROCESS_MAPPING_FAILED;
        }

    }

    return SUCCESS;
}

void process_switch(struct process* _process){
    current_process = _process;
}

static int8_t process_map_memory(struct process* _process){

    // if file type is binary
    if(_process->file_type == PROCESS_BIN_FILE){
        int8_t process_map_binary_res = process_map_binary(_process);
        if(process_map_binary_res < 0) return process_map_binary_res;
    }

    // if file type is elf (executable linker format)
    if(_process->file_type == PROCESS_ELF_FILE){
        int8_t process_map_elf_res = process_map_elf(_process);
        if(process_map_elf_res < 0) return process_map_elf_res;
    }

    // we will map stack in the reverse order because we are using a full descending stack. ie, it grows downwards
    int8_t process_map_stack_res = paging_map_range(_process->task->task_space, (void*)TASK_DEFAULT_STACK_END,_process->stack, (void*)paging_align_to_page_upper((uint32_t) _process->stack + TASK_STACK_SIZE),  PAGE_PRESENT | PAGE_USER_ACCESS | PAGE_WRITE_ACCESS);
    if(process_map_stack_res < 0) return process_map_stack_res;

    return SUCCESS; 

}

struct process* process_new(const char* filename){

    if(!processes_initialised){
        process_init();
    }

    struct process* _process = kzalloc(sizeof(struct process));

    if(!_process){
        return 0;
    }

    _process->pid = process_get_free_process_id();

    if(_process->pid == -ERR_MAX_PROCESS_LIMIT_REACHED){
        kfree(_process);
        return 0;
    }

    strcpy(_process->file, filename);
    _process->stack = kzalloc(PROCESS_MAX_PROCESS_STACK_SIZE);

    if(!_process->stack){
        kfree(_process);
        return 0;
    }

    
    if(process_load_data(_process) < 0){
        kfree(_process->stack);
        kfree(_process);
        return 0;
    }

    _process->task = new_task(_process);

    if(!_process->task){
        kfree(_process->stack);
        kfree(_process);
        return 0;
    }


    if(process_map_memory(_process) < 0){
        kfree(_process->stack);
        close_task(_process->task);
        kfree(_process);
        return 0;
    }
    process_list[_process->pid] = _process;




    if(!current_process){
        current_process = _process;
    }


    return _process;


}

void* process_malloc(uint32_t size){

    struct process* _process = process_current();

    for(int allocation=0; allocation < PROCESS_MAX_PROCESS_MEM_ALLOCATIONS; allocation++){
        if(_process->mem_allocations[allocation] == 0){
            // if we find a free slot to place our allocation,
            void* base = kzalloc(size);
            _process->mem_allocations[allocation] = base;

            return base;
        }
    }

    // we come to this part of the code if we couldn't find any allocation.
    // so we basically return NULL. 

    return 0;

}

void* process_free(void* base_addr){
    struct process* _process = process_current();

    for(int allocation=0; allocation < PROCESS_MAX_PROCESS_MEM_ALLOCATIONS; allocation++){
        if(_process->mem_allocations[allocation] == base_addr){
            // we have found the allocation which we did.
            // we need to make the block available for future allocation for the same base address.
            _process->mem_allocations[allocation] = 0;

            kfree(base_addr);

            return base_addr;

        }
    }

    // if we reach this part of the code, we tried to free some address which we never allocated.
    // we cant help it, just return NULL.

    return 0; 

}