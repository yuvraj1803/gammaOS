#include "g-loader.h"
#include "elf/elf.h"
#include "../kernel/kstatus.h"
#include "../fs/file.h"
#include "../mm/heap/kheap.h"
#include "../string/string.h"

static void gloader_process_program_header_PTLOAD(ELF_FILE* elf_file, Elf32_Phdr* phdr){

    // virtual address to load process
    elf_file->vbase = (void*) phdr->p_vaddr;
    elf_file->vend  = (void*)((uint32_t)phdr->p_vaddr + phdr->p_filesz);

    // physical address to load process
    elf_file->pbase = (void*)((uint32_t) elf_file->data + phdr->p_offset);
    elf_file->pend  = (void*)((uint32_t) elf_file->data + phdr->p_offset + phdr->p_filesz);

}

static int gloader_process_program_headers(ELF_FILE* elf_file){

    Elf32_Ehdr* ehdr = elf_file->data;

    for(int nphdr = 0; nphdr < ehdr->e_phnum; nphdr++){
        Elf32_Phdr * phdr = ELF_get_Phdr(elf_file, nphdr);

        if(phdr -> p_type == PT_LOAD){
           gloader_process_program_header_PTLOAD(elf_file, phdr);     
        }

    }

    return SUCCESS;

}

int gloader_load_elf(char* filename, ELF_FILE** elf_file_out){

    int fd = fopen(filename, 'r');

    if(fd < 0){
        return -ERR_FS_FOPEN_FAILED;
    }

    ELF_FILE* elf_file = (ELF_FILE*) kzalloc(sizeof(ELF_FILE));

    if(!elf_file){
        fclose(fd);
        return -ERR_MALLOC_FAIL;
    }

    strncpy(elf_file->filename, filename, strlen(filename));
    
    struct file_stat elf_stat ;
    fstat(fd, &elf_stat);

    // this is going to be the physical address of elf file data
    void* elf_data = kzalloc(elf_stat.size);


    if(!elf_data){
        kfree(elf_file);
        fclose(fd);

        return -ERR_MALLOC_FAIL;
    }


    // load the file data into memory
    if(fread(elf_data, elf_stat.size ,1, fd) < 0){
        kfree(elf_file);
        kfree(elf_data);
        fclose(fd);

        return -ERR_FS_FREAD_FAILED;
    }

    // checking elf signatures, and other flags...
    if(!ELF_validate_elf_file(elf_file)){
        kfree(elf_file);
        kfree(elf_data);
        fclose(fd);

        return -ERR_INVALID_ELF_FILE;
    }


    // gotta initialise vaddr, paddr variables in the elf file.
    // these can be obtained from the program header LOAD in it.
    if(gloader_process_program_headers(elf_file) < 0){
        kfree(elf_file);
        kfree(elf_data);
        fclose(fd);

        return -ERR_ELF_PHDR_FAIL;

    }


    *elf_file_out = elf_file;


    return SUCCESS;

}