/*
    vmm.h
    Copyright Shreyas Lad (PenetratingShot) 2020

    Virtual Memory Manager
*/

#include <mm/vmm.h>

uint64_t* getPML4() {
    uint64_t* addr;
    asm volatile("movq %%cr3, %0;" : "=r"(addr));
    return (uint64_t*)addr;
}

void setPML4(uint64_t* PML4) {
    asm volatile("movq %0, %%cr3;" ::"r"(PML4) : "memory");
}

void invlpg(uint64_t* vaddr) {
    asm volatile("invlpg (%0);" ::"r"(vaddr) : "memory");
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

// maps a virtual address to a physical address
void vmap(uint64_t* vaddr, uint64_t* paddr) {
    offset_t offset = vtoof(vaddr);

    uint64_t* pml4ptr = getPML4(); // the pml4 is already created in the bootloader
    uint64_t* pml3ptr = NULL;
    uint64_t* pml2ptr = NULL;

    if ((pml4ptr[offset.pml4off] & TABLEPRESENT) == 1) {
        pml3ptr = (uint64_t*)(pml4ptr[offset.pml4off] & RMFLAGS);

        if ((pml3ptr[offset.pml3off] & TABLEPRESENT) == 1) {
            pml2ptr = (uint64_t*)(pml3ptr[offset.pml3off] & RMFLAGS);

            if ((pml2ptr[offset.pml2off] & TABLEPRESENT) == 1) {
                pml2ptr[offset.pml2off] = (uint64_t)paddr | TABLEPRESENT | TABLEWRITE | TABLEHUGE;
                invlpg(vaddr);
            } else {
                pml2ptr[offset.pml2off] = (uint64_t)paddr | TABLEPRESENT | TABLEWRITE | TABLEHUGE;
                invlpg(vaddr);
            }
        } else {
            pml2ptr = pmalloc(TABLESIZE);
            pml3ptr[offset.pml3off] = (uint64_t)pml2ptr | TABLEPRESENT | TABLEWRITE;

            pml2ptr[offset.pml2off] = (uint64_t)paddr | TABLEPRESENT | TABLEWRITE | TABLEHUGE;
            invlpg(vaddr);
        }
    } else { 
        pml3ptr = pmalloc(TABLESIZE);
        pml4ptr[offset.pml4off] = (uint64_t)pml3ptr | TABLEPRESENT | TABLEWRITE;

        pml2ptr = pmalloc(TABLESIZE);
        pml3ptr[offset.pml2off] = (uint64_t)pml2ptr | TABLEPRESENT | TABLEWRITE;

        pml2ptr[offset.pml2off] = (uint64_t)paddr | TABLEPRESENT | TABLEWRITE | TABLEHUGE;
        invlpg(vaddr);
    }
}

void vfree(uint64_t* vaddr, size_t bytes) {
    offset_t offset = vtoof(vaddr);
    uint64_t* pml4ptr = getPML4();

    uint64_t pages = bytes/PAGESIZE;

    uint64_t* pml3ptr = (uint64_t*)(pml4ptr[offset.pml4off] & RMFLAGS);
    uint64_t* pml2ptr = (uint64_t*)(pml3ptr[offset.pml3off] & RMFLAGS);

    for (uint64_t i = offset.pml2off; i < pages; i++) {
        pml2ptr[i] = 0; // TODO: free page table if necessary
    }
}