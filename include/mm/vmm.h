/*
    vmm.h
    Copyright Shreyas Lad (PenetratingShot) 2020

    Virtual Memory Manager
*/

#pragma once

#include <stdint.h>
#include <stddef.h>
#include <mm/pmm.h>

#define ENTRYSIZE 0x1000

typedef struct {
    uint8_t pml1off;
    uint8_t pml2off;
    uint8_t pml3off;
    uint8_t pml4off;
} offset_t;

void setPML4(uint64_t* PML4);
void invlpg(uint64_t vaddr);
void tlbflush();

void bootstrap();

offset_t vtoof(uint64_t* vaddr); // virtual address to offset
uint64_t* oftov(offset_t offset); // offset to virtual address

void map(uint64_t* vaddr, uint64_t* paddr);