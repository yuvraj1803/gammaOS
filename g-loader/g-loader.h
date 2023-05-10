/*
 *
 * Copyright (c) 2023 Yuvraj Sakshith <ysakshith@gmail.com>
 *
 */


#ifndef __G_LOADER_H__
#define __G_LOADER_H__

#include "elf/elf.h"

int gloader_load_elf(char* filename, ELF_FILE** elf_file_out);


#endif