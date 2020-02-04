/*
    bitmap.h
    Copyright Shreyas Lad (PenetratingShot) 2020

    Interations with the bitmap
*/

#pragma once

#include <stdint.h>
#include <stddef.h>
#include <function.h>
#include "../../multiboot.h"

#define MEMBASE 0x100000
#define PAGESIZE 2000000 // 2mb pages

#define sbit(map, index) (map[index / 8] |= (1 << (index % 8)))
#define cbit(map, index) (map[index / 8] &= ~(1 << (index % 8)))
#define rbit(row, index) ((row >> index) & 1)

// if travel is greater than (64 - index), calculate new offset with (index + travel) - 64 and move to next entry
struct index_t {
    uint64_t index;
    uint64_t travel;
};

extern uint64_t __kernel_end;
extern uint64_t totalmem;

uint64_t* bitmap = (uint64_t*)&__kernel_end;

void initMem(multiboot_info_t* mbd);
struct index_t* getFreeIndicies();