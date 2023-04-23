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

    _task->registers.ip =  TASK_DEFAULT_START;
    _task->registers.esp = TASK_DEFAULT_STACK_BEGIN;
    _task->registers.ss = USER_DATA_SELECTOR;
    _task->registers.cs = USER_CODE_SELECTOR;
    _task->process = _process;

    return SUCCESS;

}

void task_switch(struct task* _task){
    cur_task = _task;
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
