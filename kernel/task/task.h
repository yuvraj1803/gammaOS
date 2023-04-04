#ifndef __TASK_H__
#define __TASK_H__

#include "../../config.h"
#include "../../mm/paging/paging.h"


struct reg{
    uint32_t eax;
    uint32_t ebx;
    uint32_t ecx;
    uint32_t edx;
    uint32_t esi;
    uint32_t edi;
    
    uint32_t esp;
    uint32_t ebp;

    uint32_t ip;

    uint32_t cs;
    uint32_t ss;

    uint32_t flags;

};

struct task{
    struct vaddr_space* task_space; // task virtual address space structure.
    struct reg registers; // task processor state.

    struct task* next_task;
    struct task* prev_task;
};

struct task* current_task();
struct task*     new_task();
struct task*    next_task();
int8_t close_task(struct task* _task);

#endif