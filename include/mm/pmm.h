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

#include <boot/multiboot.h>

void memcpy(uint8_t* source, uint8_t* dest, uint32_t nbytes);
void memset(uint64_t* str, uint64_t c, uint64_t n);

/* Physical Memory Allocation */
void* pmalloc(uint64_t bytes);
void pmfree(void* ptr, uint64_t bytes);