/*
 *
 * Copyright (c) 2023 Yuvraj Sakshith <ysakshith@gmail.com>
 *
 */


#ifndef __KEYBOARD_H__
#define __KEYBOARD_H__

#include <stdint.h>

struct keyboard{

    int (*init)();
    char name[32];
    struct keyboard* next;
    uint8_t caps_lock;

};


void keyboard_init();
void keyboard_backspace();
void keyboard_put(char ch);
char keyboard_get();
#endif