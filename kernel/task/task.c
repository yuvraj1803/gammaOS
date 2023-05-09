/*
 *
 * Copyright (c) 2023 Yuvraj Sakshith <ysakshith@gmail.com>
 *
 */

#include "task.h"
#include "../kstatus.h"
#include "../../drivers/display/vga/vga.h"
#include "../config.h"
#include "../../mm/heap/kheap.h"
#include "../idt/idt.h"
#include "../../mm/paging/paging.h"
#include "../../string/string.h"

struct task* cur_task;       // pointer to currently running task
struct task* task_list_head; // pointer to head of linked list of tasks
struct task* task_list_tail; // pointer to tail of linked list of tasks

static void task_list_add(struct task* _task){
    if(task_list_head == 0x0){
        task_list_head = _task;
        task_list_tail = _task;
        cur_task = _task;
    }else{
        task_list_tail->next_task = _task;
        _task->prev_task = task_list_tail;
        task_list_tail = task_list_tail->next_task;
    }
}

static void task_list_delete(struct task* _task){
    if(_task == task_list_head){
        task_list_head = task_list_head->next_task;
    }else if(_task == task_list_tail){
        task_list_tail = task_list_tail->prev_task;
    }else{
        _task->prev_task->next_task = _task->next_task;
        _task->next_task->prev_task = _task->prev_task;
    }

    if(_task == cur_task) cur_task = _task->next_task; // if the current task is being removed.
    kfree(_task);
}

struct task* next_task(){
    if(cur_task->next_task == 0x0){
        return task_list_head;
    }

    return cur_task->next_task;
}


static int8_t init_task(struct task* _task, struct process* _process){
    _task->task_space = create_virtual_address_space(PAGE_PRESENT | PAGE_USER_ACCESS);
    if(_task->task_space == 0x0) return -ERR_TASK_FAIL;

    // if the input file is an elf file, we change the entry point to 'e_entry' definded in the file's elf header.
    _task->registers.ip     = TASK_DEFAULT_START;
    _task->registers.esp    = TASK_DEFAULT_STACK_BEGIN;
    _task->registers.ss     = USER_DATA_SELECTOR;
    _task->registers.cs     = USER_CODE_SELECTOR;
    _task->process          = _process;

    return SUCCESS;

}

void task_switch(struct task* _task){
    cur_task = _task;

    /*
    Note: we are merely switching the the task address space. The data selectors are still of the kernel's
    */
    
    load_page_directory(_task->task_space->pd);
}

void change_to_current_task_page_directory(){
    set_all_segments_to_user_data_segment();
    load_page_directory(cur_task->task_space->pd);
}

void change_to_given_task_page_directory(struct task* _task){
    set_all_segments_to_user_data_segment();
    load_page_directory(_task->task_space->pd);
}

void switch_current_page_directory_to_current_task_pd(){
    set_all_segments_to_user_data_segment();
    task_switch(cur_task);
}

/**==========================================================================*/
void gammaos_first_ever_task(){
    if(!cur_task){
        kpanic("gammaos_first_ever_task: first task not provided!\n");
    }

    task_switch(task_list_head);
    enter_task(&cur_task->registers);

}

/**==========================================================================*/

void current_task_save_state(struct interrupt_frame* iframe){

    if(!cur_task){
        kpanic("No task running!\n");
    }

    // save processor state 

    cur_task->registers.cs = iframe->cs;
    cur_task->registers.ss = iframe->ss;
    cur_task->registers.eax = iframe->eax;
    cur_task->registers.ebx = iframe->ebx;
    cur_task->registers.ecx = iframe->ecx;
    cur_task->registers.edx = iframe->edx;
    cur_task->registers.esi = iframe->esi;
    cur_task->registers.edi = iframe->edi;
    cur_task->registers.esp = iframe->esp;
    cur_task->registers.ip = iframe->ip;
    cur_task->registers.flags = iframe->flags;
    cur_task->registers.ebp = iframe->ebp;
        

}


struct task* current_task(){
    return cur_task;
}

struct task* new_task(struct process* _process){
    struct task* _task = (struct task*) kzalloc(sizeof(struct task));
    if(_task == 0x0){
        return 0x0; // couldn't allocate memory for the task
    }

    if(init_task(_task, _process) == -ERR_TASK_FAIL) return 0x0; // failed to initialise task.

    task_list_add(_task);

    return _task;
}

int8_t close_task(struct task* _task){

    destroy_virtual_address_space(_task->task_space);
    task_list_delete(_task);

    kfree(_task);

    return SUCCESS;
}


/*
    Given some task and its address space, this function can move the data from the process' address space
    to the kernel address space.
*/
int copy_data_from_task_to_kernel(struct task* _task, void* addr_in_task, void* addr_in_kern, uint32_t size){

    if(size > PAGE_SIZE){
        return -ERR_INVARG;
    }

    uint8_t* shared_mem_base = (uint8_t*) kzalloc(size);

    if(!shared_mem_base){
        return -ERR_MALLOC_FAIL;
    }

    uint32_t old_pt_entry = paging_virt_to_phy(_task->task_space, shared_mem_base);

    // map virtual address of shared mem in task
    // address space directly to physical memory
    // this is done because, we want the kernel and the task
    // to see the same memory
    map_vaddr_to_val(_task->task_space->pd, shared_mem_base, (uint32_t) shared_mem_base | PAGE_PRESENT | PAGE_WRITE_ACCESS | PAGE_USER_ACCESS);
    load_page_directory(_task->task_space->pd);
    strncpy((char*)shared_mem_base, (char*)addr_in_task, size);

    change_to_kernel_page_directory();

    // change the shared mem base to wherever it pointed before
    map_vaddr_to_val(_task->task_space->pd, shared_mem_base, old_pt_entry);

    int bytes_written = strncpy((char*)addr_in_kern, (char*)shared_mem_base, size);

    kfree(shared_mem_base);

    return bytes_written;
    

} 

/*access an element from task's stack*/
void* task_access_stack(struct task* _task, int stack_index){

    uint32_t* task_esp = (uint32_t*) _task->registers.esp;

    change_to_given_task_page_directory(_task);

    void* stack_elem = (void*) task_esp[stack_index];

    change_to_kernel_page_directory();

    return stack_elem;


}