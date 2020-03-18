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
#include <mm/mem.h>
#include <boot/multiboot.h>

extern uint64_t _kernel_end;

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
void* pmrealloc(void* ptr, size_t oldSize, size_t newSize);