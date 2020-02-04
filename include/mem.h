#pragma once

#include <stdint.h>
#include <stddef.h>
#include <function.h>
#include "../cpu/type.h"

#include "../multiboot.h"

#define sbit(map, index) (map[index / 8] |= (1 << (index % 8)))
#define cbit(map, index) (map[index / 8] &= ~(1 << (index % 8)))
#define rbit(row, index) ((row >> index) & 1)

#define MEMBASE 0x100000
#define PAGESIZE 2000000 // 2mb pages

extern uint64_t __kernel_end;
extern uint64_t totalmem;

void memcpy(uint8_t* source, uint8_t* dest, uint32_t nbytes);
void memset(uint64_t* dest, uint64_t val, uint64_t len);

void initMem(multiboot_info_t* mbd);

/* Physical Memory Allocation */
size_t* palloc(size_t bytes);
void pfree(void* ptr);