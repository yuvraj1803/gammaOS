#include "g-loader.h"
#include "elf/elf.h"
#include "../mm/memory.h"
#include "../config.h"
#include "../kernel/kstatus.h"


const char ELF_MAGIC_NUMBER[4] = {0x7F,0x45,0x4C,0x46};


static uint8_t ELF_validate_signature(void* data){
    return memcmp(data, (void*) ELF_MAGIC_NUMBER, 4) == 0;
}

static uint8_t ELF_validate_class32(Elf32_Ehdr* hdr){
    return hdr->e_ident[EI_CLASS] == ELFCLASS32;
}

static uint8_t ELF_validate_little_endian(Elf32_Ehdr* hdr){
    return hdr->e_ident[EI_DATA] == ELFDATA2LSB;
}

static uint8_t ELF_validate_executable(Elf32_Ehdr* hdr){
    /*
    'hdr->e_entry >= TASK_DEFAULT_START' wont allow program to be loaded anywhere below 0x400000 (security reasons)
    */
    return hdr->e_type == ET_EXEC && hdr->e_entry >= TASK_DEFAULT_START;
}

static uint8_t ELF_validate_Phdr(Elf32_Ehdr* hdr){
    return hdr->e_phoff != 0;
}

static uint8_t ELF_validate_Shdr(Elf32_Ehdr* hdr){
    return hdr->e_shoff != 0;
}

static Elf32_Ehdr* ELF_get_Ehdr(ELF_FILE* file){
    return file->data;
}

// returns 'index'th program header
static Elf32_Phdr* ELF_get_Phdr(ELF_FILE* file, uint32_t index){
    return &((Elf32_Phdr*)((uint32_t) ELF_get_Ehdr(file) + ELF_get_Ehdr(file)->e_phoff))[index];
}

// returns 'index'th section header
static Elf32_Shdr* ELF_get_Shdr(ELF_FILE* file, uint32_t index){
    return &((Elf32_Shdr*)((uint32_t) ELF_get_Ehdr(file) + ELF_get_Ehdr(file)->e_shoff))[index];
}

// returns the string table address
static char* ELF_get_string_table(ELF_FILE* file){

    Elf32_Ehdr* ehdr = ELF_get_Ehdr(file);
    Elf32_Shdr* string_table_shdr = ELF_get_Shdr(file, ehdr->e_shstrndx);

    return (char*) ehdr + string_table_shdr->sh_offset;
}

uint8_t ELF_validate_elf_file(ELF_FILE* file){

    (

        ELF_validate_signature(file->data)                  &&
        ELF_validate_class32(ELF_get_Ehdr(file))            &&
        ELF_validate_executable(ELF_get_Ehdr(file))         &&
        ELF_validate_little_endian(ELF_get_Ehdr(file))      &&
        ELF_validate_Phdr(ELF_get_Ehdr(file))               &&
        ELF_validate_Shdr(ELF_get_Ehdr(file))

    ) != 0 ? SUCCESS : -ERR_INVALID_ELF_FILE;

}