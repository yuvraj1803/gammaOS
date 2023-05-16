/*
 *
 * Copyright (c) 2023 Yuvraj Sakshith <ysakshith@gmail.com>
 *
 */


#include "stdio.h"
#include "unistd.h"

void gshell_init(){
    printf("gammaOS v1.0.0\n");
}

void gshell_process_command(char* command){
    if(exec(command) < 0){
        printf("couldnt open file\n");
    }
}

int main(){


    gshell_init();

    while(1){
        printf("> ");
        
        char command[30];
        gets(command);

        gshell_process_command(command);

    }


    while(1){}

    return 0;
}