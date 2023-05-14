/*
    
 *
 * Copyright (c) 2023 Yuvraj Sakshith <ysakshith@gmail.com>
 *

    gammaOS init file

*/

#include "../lib/include/stdio.h"
#include "../lib/include/stdlib.h"


int main(){

    void* mem = malloc(10);

    free(mem);

    while(1){}

    return 0;
}