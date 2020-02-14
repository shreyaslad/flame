/*
    pmm.h
    Copyright Shreyas Lad (PenetratingShot) 2020

    Physical Memory Manager
*/

#pragma once

#include <stdint.h>
#include <stddef.h>
#include <function.h>
#include <mm/bitmap.h>
#include <mm/bit.h>
#include <boot/multiboot.h>

void memcpy(uint8_t* source, uint8_t* dest, uint32_t nbytes);
void memset(void* dest, int val, size_t len);

/* Physical Memory Allocation */
uint64_t* pmalloc(size_t bytes);
void pmfree(void* ptr, size_t bytes);