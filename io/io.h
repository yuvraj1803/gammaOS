#ifndef __IO_H__
#define __IO_H__

#include <stdint.h>

uint8_t insb(uint16_t port);
uint16_t insw(uint16_t port);
uint32_t insdw(uint16_t port);

void outb(uint16_t port, uint8_t val);
void outw(uint16_t port, uint16_t val);
void outdw(uint16_t port, uint32_t val);


#endif