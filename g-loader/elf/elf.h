/*
 *
 * Copyright (c) 2023 Yuvraj Sakshith <ysakshith@gmail.com>
 *
 */


#ifndef __G_LOADER_ELF_H__
#define __G_LOADER_ELF_H__

/*

    all the information of constants defined below can be found at:
    https://refspecs.linuxfoundation.org/elf/elf.pdf
*/

#include <stdint.h>
#include "../config.h"

#define PF_X 0x01
#define PF_W 0x02
#define PF_R 0x04

#define PT_NULL 0
#define PT_LOAD 1
#define PT_DYNAMIC 2
#define PT_INTERP 3
#define PT_NOTE 4
#define PT_SHLIB 5
#define PT_PHDR 6

#define SHT_NULL 0
#define SHT_PROGBITS 1
#define SHT_SYMTAB 2
#define SHT_STRTAB 3
#define SHT_RELA 4
#define SHT_HASH 5
#define SHT_DYNAMIC 6
#define SHT_NOTE 7
#define SHT_NOBITS 8
#define SHT_REL 9
#define SHT_SHLIB 10
#define SHT_DYNSYM 11
#define SHT_LOPROC 12
#define SHT_HIPROC 13
#define SHT_LOUSER 14
#define SHT_HIUSER 15

#define ET_NONE 0
#define ET_REL 1
#define ET_EXEC 2
#define ET_DYN 3
#define ET_CORE 4

#define EI_NIDENT 16
#define EI_CLASS 4
#define EI_DATA 5

#define ELFCLASSNONE 0
#define ELFCLASS32 1
#define ELFCLASS64 2

#define ELFDATANONE 0
#define ELFDATA2LSB 1
#define ELFDATA2MSB 2

#define SHN_UNDEF 0

typedef uint16_t Elf32_Half;
typedef uint32_t Elf32_Word;
typedef int32_t Elf32_Sword;
typedef uint32_t Elf32_Addr;
typedef int32_t Elf32_Off;


// Program Header
typedef struct
{
    Elf32_Word  p_type;
    Elf32_Off   p_offset;
    Elf32_Addr  p_vaddr;
    Elf32_Addr  p_paddr;
    Elf32_Word  p_filesz;
    Elf32_Word  p_memsz;
    Elf32_Word  p_flags;
    Elf32_Word  p_align;
} __attribute__((packed)) Elf32_Phdr;

// Section Header
typedef struct
{
    Elf32_Word  sh_name;
    Elf32_Word  sh_type;
    Elf32_Word  sh_flags;
    Elf32_Addr  sh_addr;
    Elf32_Off   sh_offset;
    Elf32_Word  sh_size;
    Elf32_Word  sh_link;
    Elf32_Word  sh_info;
    Elf32_Word  sh_addralign;
    Elf32_Word  sh_entsize;
} __attribute__((packed)) Elf32_Shdr;

// ELF Header
typedef struct
{
    unsigned char   e_ident[EI_NIDENT];
    Elf32_Half      e_type;
    Elf32_Half      e_machine;
    Elf32_Word      e_version;
    Elf32_Addr      e_entry;
    Elf32_Off       e_phoff;
    Elf32_Off       e_shoff;
    Elf32_Word      e_flags;
    Elf32_Half      e_ehsize;
    Elf32_Half      e_phentsize;
    Elf32_Half      e_phnum;
    Elf32_Half      e_shentsize;
    Elf32_Half      e_shnum;
    Elf32_Half      e_shstrndx;
} __attribute__((packed)) Elf32_Ehdr;

// Dynamic Structure
typedef struct
{
    Elf32_Sword     d_tag;
    union
    {
        Elf32_Word  d_val;
        Elf32_Addr  d_ptr;
    } d_un;
} __attribute__((packed)) Elf32_Dyn ;

// Symbol Table Entry
typedef struct
{
    Elf32_Word      st_name;
    Elf32_Addr      st_value;
    Elf32_Word      st_size;
    unsigned char   st_info;
    unsigned char   st_other;
    Elf32_Half      st_shndx;
} __attribute__((packed)) Elf32_Sym;


typedef struct{

    char filename[FS_MAX_PATH_LENGTH];
    uint32_t size; // file size in bytes

    void* data; // file data


    // base and end addresses in process virtual memory
    void* vbase;
    void* vend;

    // base and end addresses in physical memory
    void* pbase;
    void* pend;


} ELF_FILE;

uint8_t                 ELF_validate_Phdr(Elf32_Ehdr* hdr);
uint8_t                 ELF_validate_Shdr(Elf32_Ehdr* hdr);
Elf32_Ehdr*             ELF_get_Ehdr(ELF_FILE* file);
Elf32_Phdr*             ELF_get_Phdr(ELF_FILE* file, uint32_t index);
Elf32_Shdr*             ELF_get_Shdr(ELF_FILE* file, uint32_t index);
char*                   ELF_get_string_table(ELF_FILE* file);
uint8_t                 ELF_validate_elf_file(ELF_FILE* file);


#endif