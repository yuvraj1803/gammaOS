#include "../include/stdio.h"
#include "../include/stdlib.h"

#include <stdarg.h>

int printf(char* fmt, ...){

    va_list ap;

    va_start(ap, fmt);

    int chars_written = 0; // this will keep a track of number of characters succesfully written to the screen
        
    for(char* p = fmt; *p; p++){

        if(*p != '%'){
            putch(*p);
            chars_written++;
        }else{

            switch(*++p){
                case 'i': // integer
                    int val = va_arg(ap, int);
                    puts(itoa(val));
                    chars_written++;
                    break;
                case 's': // string
                    char* str = va_arg(ap, char*);
                    puts(str);
                    chars_written++;
                    break;
                default: // unknown
                    break;
            }


        }
        
    }

    return chars_written;

}