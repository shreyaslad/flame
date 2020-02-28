/*
    vmm.h
    Copyright Shreyas Lad (PenetratingShot) 2020

    Virtual Memory Manager
*/

#pragma once

#include <stdint.h>
#include <stddef.h>
#include <mm/pmm.h>

#define TABLESIZE 0x1000

#define RMFLAGS 0x000FFFFFFFFFF000
#define TABLEPRESENT (1 << 0)
#define TABLEWRITE (1 << 1)
#define TABLEHUGE (1 << 7)

typedef struct {
    uint8_t pml1off;
    uint8_t pml2off;
    uint8_t pml3off;
    uint8_t pml4off;
} offset_t;

void setPML4(uint64_t* PML4);
void invlpg(uint64_t* vaddr);
void tlbflush();

uint64_t* getpaddr(void* vaddr);
offset_t vtoof(uint64_t* vaddr); // virtual address to offset

void vmap(uint64_t* vaddr, uint64_t* paddr);
void vfree(uint64_t* vaddr, size_t bytes);