/*
 *
 * Copyright (c) 2023 Yuvraj Sakshith <ysakshith@gmail.com>
 *
 */

#ifndef __TASK_H__
#define __TASK_H__

#include "../../config.h"
#include "../../mm/paging/paging.h"
#include "process/process.h"
#include "../idt/idt.h"



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
struct process;
struct interrupt_frame;
struct task{
    struct vaddr_space* task_space; // task virtual address space structure.
    struct reg registers; // task processor state.

    struct process* process;

    struct task* next_task;
    struct task* prev_task;
};

struct task*    current_task();
struct task*    new_task(struct process* _process);
struct task*    next_task();
int8_t          close_task(struct task* _task);

void enter_task(struct reg* registers);
void restore_gprs(struct reg* registers);
void set_all_segments_to_user_data_segment();
void change_to_current_task_page_directory();
void current_task_save_state();


void gammaos_first_ever_task();
#endif