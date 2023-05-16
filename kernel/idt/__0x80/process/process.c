#include "process.h"
#include "../../../kernel.h"
#include "../../../task/task.h"
#include "../../../task/process/process.h"
#include "../../../kstatus.h"
#include "../../idt.h"
#include "../../../../config.h"

void* __0x80_EXEC(struct interrupt_frame* iframe){
    char* filename_in_taskspace = (char*) task_access_stack(current_task(), 0);

    char filename_in_kernel[FS_MAX_PATH_LENGTH];

    copy_data_from_task_to_kernel(current_task(), (void*)filename_in_taskspace, (void*) filename_in_kernel, FS_MAX_PATH_LENGTH);

    struct process* _process = process_new(filename_in_kernel   );

    if(_process == 0){
        return (void*) -ERR_PROCESS_CREATION_FAILED;
    }

    task_switch(_process->task);
    enter_task(&_process->task->registers);

    return (void*) SUCCESS;

}