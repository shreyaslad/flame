/*  
    pmm.h
    Copyright Shreyas Lad (PenetratingShot) 2020

    Physical Memory Manager
*/

#pragma once

#include <stdint.h>
#include <stddef.h>
#include <function.h>
#include <mm/bit.h>
#include <boot/multiboot.h>

#define MEMBASE 0x100000
#define PAGESIZE 2000000 // 2mb pages

extern uint64_t __kernel_end;

extern uint64_t* bitmap;
extern uint64_t totalmem;
extern uint64_t bitmapEntries;

typedef struct {
    uint64_t entry;
    uint64_t bit;
} bitloc_t;

void memcpy(uint8_t* source, uint8_t* dest, uint32_t nbytes);
void memset(void* dest, int val, size_t len);

/* Physical Memory Allocation */
void* pmalloc(size_t pages);
void pmfree(void* ptr, size_t pages);