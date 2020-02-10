/*
    vmm.h
    Copyright Shreyas Lad (PenetratingShot) 2020

    Virtual Memory Manager
*/

#include <mm/vmm.h>

uint64_t* getPML4() {
    uint64_t* addr;
    asm volatile("movq, %%cr3, %0;" : "=r"(addr));
    return (uint64_t*)addr;
}

void setPML4(uint64_t* PML4) {
    asm volatile("movq %0, %%cr3;" ::"r"(PML4) : "memory");
}

void invlpg(uint64_t vaddr) {
    asm volatile("inlvpg (%0);" ::"r"(vaddr) : "memory");
}

void tlbflush() {
    setPML4(getPML4());
}

// virtual address to offset
offset_t vtoof(uint64_t* vaddr) {
    uint64_t addr = (uint64_t)vaddr;

    offset_t offset = {
        .pml4off = (uint8_t)((addr & ((uint64_t)0x1ff << 39)) >> 39),
        .pml3off = (uint8_t)((addr & ((uint64_t)0x1ff << 39)) >> 39),
        .pml2off = (uint8_t)((addr & ((uint64_t)0x1ff << 39)) >> 39),
        .pml1off = (uint8_t)((addr & ((uint64_t)0x1ff << 39)) >> 39)
    };

    return offset;
}

// offset to virtual address
uint64_t* oftov(offset_t offset) {
    uint64_t addr = 0;

    addr |= offset.pml4off << 39;
    addr |= offset.pml3off << 30;
    addr |= offset.pml2off << 21;
    addr |= offset.pml1off << 12;

    return (uint64_t*)addr;
}

// maps a virtual address to a physical address
void map(uint64_t* vaddr, uint64_t* paddr) {
    offset_t offset = vtoof(vaddr);

    uint64_t pmladdr = getPML4(); // the pml4 is already created in the bootloader
} 