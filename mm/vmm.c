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

void tlbflush() { setPML4(getPML4()); }

// virtual address to offset
offset_t vtoof(uint64_t* vaddr) {
  size_t aa = (size_t)vaddr;

  offset_t offset = {
      .pml4off = (aa & ((size_t)0x1ff << 39)) >> 39,
      .pml3off = (aa & ((size_t)0x1ff << 30)) >> 30,
      .pml2off = (aa & ((size_t)0x1ff << 21)) >> 21,
      .pml1off = (aa & ((size_t)0x1ff << 12)) >> 12,
  };

  return offset;
}

// grabs the physical address from an existing virtual address in the page
// tables
uint64_t* getpaddr(void* vaddr) {
  offset_t offset = vtoof((uint64_t*)vaddr);

  uint64_t* pml4ptr = getPML4();
  uint64_t* pml3ptr = pml4ptr[offset.pml4off] & RMFLAGS;
  uint64_t* pml2ptr = pml3ptr[offset.pml3off] & RMFLAGS;

  return pml2ptr[offset.pml2off] & RMFLAGS;
}

// maps a virtual address to a physical address
void vmap(uint64_t* vaddr, uint64_t* paddr, uint64_t* pml4ptr,
          uint64_t permission) {
  offset_t offset = vtoof(vaddr);

  uint64_t pml3phys = NULL;
  uint64_t* pml3virt = NULL;
  uint64_t pml2phys = NULL;
  uint64_t* pml2virt = NULL;

  if ((pml4ptr[offset.pml4off] & TABLEPRESENT) == 1) {
    pml3phys = (uint64_t)(pml4ptr[offset.pml4off] & RMFLAGS);
    pml3virt = (uint64_t)(pml3phys + HIGH_VMA);
  } else {
    pml3phys = pmalloc(1);
    pml3virt = (uint64_t)(pml3phys + HIGH_VMA);
    pml4ptr = (uint64_t)pml3phys | TABLEPRESENT | TABLEWRITE;
  }

  if ((pml3virt[offset.pml3off] & TABLEPRESENT) == 1) {
    pml2phys = (uint64_t)(pml3virt[offset.pml3off] & RMFLAGS);
    pml2virt = (uint64_t)(pml2phys + HIGH_VMA);
  } else {
    pml2phys = pmalloc(1);
    pml2virt = (uint64_t)(pml2phys + HIGH_VMA);
    pml3virt = (uint64_t)pml2phys | TABLEPRESENT | TABLEWRITE;
  }

  if (permission == SUPERVISOR) {
    pml2virt[offset.pml2off] =
        (uint64_t)paddr | TABLEPRESENT | TABLEWRITE | TABLEHUGE;
  } else {
    pml2virt[offset.pml2off] =
        (uint64_t)paddr | TABLEPRESENT | TABLEWRITE | TABLEHUGE | TABLEUSER;
  }

  sprintf("PML3: %x | PML2: %x\n", (uint64_t)pml4ptr, (uint64_t)pml3virt,
          (uint64_t)pml2virt);

  invlpg(vaddr);
}

void test() {
  offset_t offset = vtoof(0);

  uint64_t* pml4ptr = (uint64_t*)((size_t)((size_t)getPML4() + HIGH_VMA));

  uint64_t pml3phys = 0x300000;
  uint64_t* pml3virt = (uint64_t*)0xFFFF800000300000;
  uint64_t pml2phys = 0x500000;
  uint64_t* pml2virt = (uint64_t*)0xFFFF800000500000;

  pml4ptr[offset.pml4off] = (uint64_t)pml3phys | TABLEPRESENT | TABLEWRITE;
  pml3virt[offset.pml3off] = (uint64_t)pml2phys | TABLEPRESENT | TABLEWRITE;

  pml2virt[offset.pml2off] =
      (uint32_t)0 | TABLEPRESENT | TABLEWRITE | TABLEHUGE;

  pml2virt[offset.pml2off + 1] =
      (uint32_t)0 | TABLEPRESENT | TABLEWRITE | TABLEHUGE;

  pml2virt[offset.pml2off + 2] =
      (uint32_t)0 | TABLEPRESENT | TABLEWRITE | TABLEHUGE;

  pml2virt[offset.pml2off + 3] =
      (uint32_t)0 | TABLEPRESENT | TABLEWRITE | TABLEHUGE;

  pml2virt[offset.pml2off + 4] =
      (uint32_t)0 | TABLEPRESENT | TABLEWRITE | TABLEHUGE;
}

void vfree(uint64_t* vaddr, size_t pages) {
  offset_t offset = vtoof(vaddr);
  uint64_t* pml4ptr = getPML4();

  uint64_t* pml3ptr = (uint64_t*)(pml4ptr[offset.pml4off] & RMFLAGS);
  uint64_t* pml2ptr = (uint64_t*)(pml3ptr[offset.pml3off] & RMFLAGS);

  for (uint64_t i = offset.pml2off; i < pages + 1; i++) {
    pml2ptr[i] = 0; // TODO: free page table if necessary
  }

  tlbflush(vaddr);
}