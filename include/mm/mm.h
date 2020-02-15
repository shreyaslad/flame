/*
    mm.h
    Copyright Shreyas Lad (PenetratingShot) 2020

    Kernel Memory Manager
*/

#pragma once

#include <mm/pmm.h>
#include <mm/vmm.h>
#include <function.h>

extern uint64_t KNL_HIGH_VMA;

void initMem(multiboot_info_t* mbd);

void* malloc(size_t bytes);
void free(void* vaddr);