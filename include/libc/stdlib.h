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
#include <boot/multiboot.h>

#define UNUSED(x) (void)(x)
#define EXIT_SUCCESS 1
#define EXIT_FAILURE 0

extern uint64_t KNL_HIGH_VMA;

void initMem(multiboot_info_t* mbd);

void memcpy(uint8_t* source, uint8_t* dest, uint32_t nbytes);
void memset(void* dest, int val, size_t len);

void* malloc(size_t bytes);
void free(void* vaddr);