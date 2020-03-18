/*
    stdlib.h
    Copyright Shreyas Lad (PenetratingShot) 2019
    Standard Library
*/

#pragma once

#include <stdint.h>
#include <stddef.h>
#include <mm/mem.h>
#include <mm/pmm.h>
#include <mm/vmm.h>
#include <mm/liballoc.h>
#include <boot/multiboot.h>

#define UNUSED(x) (void)(x)
#define EXIT_SUCCESS 1
#define EXIT_FAILURE 0

extern uint64_t _kernel_start;
extern uint64_t _kernel_end;

void initMem(multiboot_info_t* mbd);

void* memcpy(void* dest, const void* src, size_t size);
void* memset(void* bufptr, int value, size_t size);
int memcmp(const void* s1, const void* s2, size_t n);

void* malloc(size_t bytes);
void free(void* vaddr);
void* realloc(void* ptr, size_t size);