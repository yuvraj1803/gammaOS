/*
 *
 * Copyright (c) 2023 Yuvraj Sakshith <ysakshith@gmail.com>
 *
 */

#ifndef __CONFIG_H__
#define __CONFIG_H__

#define TOTAL_INTERRUPTS                    512
#define GDT_ENTRIES                         6

#define KERNEL_CODE_SELECTOR                0x08
#define KERNEL_DATA_SELECTOR                0x10

#define USER_DATA_SELECTOR                  0x23
#define USER_CODE_SELECTOR                  0x1B

#define KERNEL_HEAP_SIZE                    104857600  // --> 100 MB Heap (in bytes)
#define KERNEL_HB_SIZE                      4096      // --> Heap Block size (in bytes)
#define KERNEL_HEAP_BASE                    0x01000000
#define KERNEL_HEAP_TABLE_BASE              0x00007E00
#define KERNEL_MAX_0x80_FUNCTIONS           256

#define MAX_NUMBER_OF_DISKS                 26
#define PRIMARY_DISK                        'A'


#define FS_TOTAL_FILESYSTEMS                16
#define FS_TOTAL_FILE_DESCRIPTORS           1024
#define FS_MAX_PATH_LENGTH                  512
#define FS_MAX_PATH_UNIT_LENGTH             64

#define TASK_DEFAULT_START                  0x400000
#define TASK_STACK_SIZE                     16384       // 16kB
#define TASK_DEFAULT_STACK_BEGIN            0x3FF000
#define TASK_DEFAULT_STACK_END              TASK_DEFAULT_STACK_BEGIN - TASK_STACK_SIZE

#define PROCESS_MAX_PROCESSES               16
#define PROCESS_MAX_PROCESS_MEM_ALLOCATIONS 1024
#define PROCESS_MAX_PROCESS_STACK_SIZE      65536
#define PROCESS_MAX_KEYBOARD_BUFFER_SIZE    1024

#define VGA           
#define PS2    
#define PRINT_KEYBOARD_INP_TO_SCREEN
#endif