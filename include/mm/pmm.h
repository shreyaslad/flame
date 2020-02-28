/*  
    pmm.h
    Copyright Shreyas Lad (PenetratingShot) 2020

    Physical Memory Manager
*/

#pragma once

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>
#include <mm/bit.h>
#include <boot/multiboot.h>

#define MEMBASE 0x100000
#define PAGESIZE 0x2000000 // 2mb pages

extern uint64_t __kernel_end;

extern uint64_t* bitmap;
extern uint64_t totalmem;
extern uint64_t bitmapEntries;

typedef struct {
    uint64_t entry;
    uint64_t bit;
} bitloc_t;

/* Physical Memory Allocation */
void* pmalloc(size_t pages);
void pmfree(void* ptr, size_t pages);