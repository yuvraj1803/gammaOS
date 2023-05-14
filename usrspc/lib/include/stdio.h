/*
 *
 * Copyright (c) 2023 Yuvraj Sakshith <ysakshith@gmail.com>
 *
 */

#ifndef __STDIO_H__
#define __STDIO_H__

void puts(const char* str);
void putch(char ch);

char getch();
char* gets(char* str);

int printf(char* fmt, ...);

#endif