/*
    
 *
 * Copyright (c) 2023 Yuvraj Sakshith <ysakshith@gmail.com>
 *

    gammaOS init file

*/

#include "../lib/include/stdio.h"


int main(){

    while(1){

        char c = getch();
        if(c == '\n') break;
        putch(c);
    }

    while(1){}

    return 0;
}