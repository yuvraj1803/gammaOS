#include "heap.h"
#include "../../../../task/task.h"
#include "../../../../task/process/process.h"

void* __0x80_MALLOC(struct interrupt_frame* iframe){
    uint32_t size = (uint32_t) task_access_stack(current_task(), 0);

    return process_malloc(size);

}

void*  __0x80_FREE  (struct interrupt_frame* iframe){
    void* base = task_access_stack(current_task(), 0);

    return (void*)process_free(base);
}