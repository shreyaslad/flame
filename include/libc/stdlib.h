/*
    stdlib.h
    Copyright Shreyas Lad (PenetratingShot) 2019
    Standard Library
*/

#pragma once

#include <stdint.h>
#include <stddef.h>
#include <mm/pmm.h>
#include <mm/vmm.h>
#include <mm/liballoc.h>
#include <boot/multiboot.h>

#define UNUSED(x) (void)(x)
#define EXIT_SUCCESS 1
#define EXIT_FAILURE 0

#define KERNEL_HIGH_VMA 0xFFFFFFFF80000000


extern uint64_t _kernel_start;
extern uint64_t _kernel_end;

void initMem(multiboot_info_t* mbd);

void memcpy(void* restrict dest, void* restrict src, uint64_t size);
void memset(void* bufptr, int value, uint64_t size);

void* malloc(size_t bytes);
void free(void* vaddr);