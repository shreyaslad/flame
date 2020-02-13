/*
    bitmap.h
    Copyright Shreyas Lad (PenetratingShot) 2020

    Interations with the bitmap
*/

#pragma once

#include <stdint.h>
#include <stddef.h>
#include <function.h>
#include <boot/multiboot.h>

#define MEMBASE 0x100000
#define PAGESIZE 2000000 // 2mb pages

#define sbit(map, index) (map[index / 8] |= (1 << (index % 8)))
#define cbit(map, index) (map[index / 8] &= ~(1 << (index % 8)))
#define rbit(row, index) ((row >> index) & 1)

typedef struct {
    uint64_t entries;
    uint64_t row;
    uint64_t bit;
} index_t;

extern uint64_t __kernel_end;

uint64_t totalmem;
uint64_t bitmapEntries;
uint64_t* bitmap;

void initMem(multiboot_info_t* mbd);
index_t* getFreeIndicies(uint64_t pages);