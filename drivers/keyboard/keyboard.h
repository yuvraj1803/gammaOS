#ifndef __KEYBOARD_H__
#define __KEYBOARD_H__

struct keyboard{

    int (*init)();
    char name[32];
    struct keyboard* next;

};


void keyboard_init();
void keyboard_backspace();
void keyboard_put(char ch);
char keyboard_get();
#endif