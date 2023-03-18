#ifndef __CONFIG_H__
#define __CONFIG_H__

#define TOTAL_INTERRUPTS 512

#define KERNEL_CODE_SELECTOR 0x08
#define KERNEL_DATA_SELECTOR 0x10

#define KERNEL_HEAP_SIZE        104857600  // --> 100 MB Heap (in bytes)
#define KERNEL_HB_SIZE          4096      // --> Heap Block size (in bytes)
#define KERNEL_HEAP_BASE        0x01000000
#define KERNEL_HEAP_TABLE_BASE  0x00007E00

#endif