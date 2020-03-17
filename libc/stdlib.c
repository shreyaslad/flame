/*
  stdlib.c
  Copyright Shreyas Lad (PenetratingShot) 2020

  Standard Library
*/

#include <libc/stdlib.h>

/* Initialization */
void initMem(multiboot_info_t* mbd) {

  uint64_t currentAddr = mbd->mmap_addr;
  uint64_t memoryTraversed = 0;

  while (memoryTraversed < mbd->mmap_length) {
    multiboot_memory_map_t* currentEntry = (multiboot_memory_map_t*)currentAddr;

    memoryTraversed += (currentEntry->size + sizeof(uint32_t));
    currentAddr += (currentEntry->size + sizeof(uint32_t));

    if (memoryTraversed == mbd->mmap_length)
      break;

    totalmem += currentEntry->len;

    sprintf("Addr: %x | Len: %d | Type: %d\n", currentEntry->addr,
            currentEntry->len, currentEntry->type);
  }

  sprintf("Total Mem: %d\n", totalmem);

  uint64_t entries = totalmem / PAGESIZE / 8;
  memset(bitmap, 0, entries);

  uint64_t* bootPML4 = (uint64_t*)((uint64_t)getPML4() + HIGH_VMA);

  for (uint64_t i = 0; i < totalmem / PAGESIZE; i++) {
    vmap(i * PAGESIZE + HIGH_VMA, i * PAGESIZE, bootPML4, SUPERVISOR);
    sprintf("Pages Mapped: %d\n", i);
  }

  uint64_t* newPML4Phys = pmalloc(1);
  uint64_t* newPML4 = (uint64_t*)((uint64_t)newPML4Phys + HIGH_VMA);
  sprintf("New PML4: %x\n", (uint64_t)newPML4);
  memset(newPML4, 0, 0x1000);

  for (uint64_t i = 0; i < totalmem / PAGESIZE; i++) {
    vmap(i * PAGESIZE + HIGH_VMA, i * PAGESIZE, newPML4, SUPERVISOR);
    sprintf("Pages Mapped: %d\n", i);
  }

  uint64_t __kernel_start_virt = __kernel_start + KNL_HIGH_VMA;
  uint64_t kernelPages = (__kernel_end - __kernel_start) / PAGESIZE + 1;

  for (uint64_t i = 0; i < kernelPages; i++) {
    vmap(KNL_HIGH_VMA + (i * PAGESIZE), __kernel_start_virt + (i * PAGESIZE),
         newPML4, SUPERVISOR);
  }

  sprintf("Setting new pml4\n");
  setPML4((uint64_t)newPML4Phys);
  sprintf("Set new pml4\n");

  // this block works perfectly fine
  /*currentAddr = mbd->mmap_addr;
  memoryTraversed = 0;

  while (memoryTraversed < mbd->mmap_length) {
    multiboot_memory_map_t* currentEntry = (multiboot_memory_map_t*)currentAddr;

    memoryTraversed += (currentEntry->size + sizeof(uint32_t));
    currentAddr += (currentEntry->size + sizeof(uint32_t));

    if (memoryTraversed == mbd->mmap_length)
      break;

    if (currentEntry->type != MULTIBOOT_MEMORY_AVAILABLE) {
      // find the page the block belongs to from the address
      // mark the page
      // move onto the next entry

      uint64_t page = currentEntry->addr / PAGESIZE;
      if ((currentEntry->addr + currentEntry->len) > PAGESIZE) {

      } else {
        setAbsoluteBit(bitmap, page);
      }
    }

    // repeat the loop
  }*/
}

void memset(void* bufptr, int value, uint64_t size) {
  unsigned char* buf = (unsigned char*)bufptr;
  for (uint64_t i = 0; i < size; i++) {
    buf[i] = value;
  }
}

void memcpy(void* restrict dest, void* restrict src, uint64_t size) {
  const unsigned char* src2 = (const unsigned char*)src;
  unsigned char* dst = (unsigned char*)dest;
  for (uint64_t i = 0; i < size; i++) {
    dst[i] = src2[i];
  }
}
/* Allocation / Deallocation */
void* malloc(size_t bytes) {
  uint64_t pages = bytes / PAGESIZE;

  // stupid hack
  if (bytes > 0 && pages == 0) {
    pages++;
  }

  uint64_t* ret = (uint64_t*)pmalloc(pages);

  vmap(ret + 0xFFFF800000000000, ret, (uint64_t*)getPML4(), SUPERVISOR);
  ret = (uint64_t*)((uint64_t)ret + 0xFFFF800000000000);

  return ret;
}

void free(void* vaddr) {
  vfree((uint64_t*)vaddr,
        PAGESIZE); // only frees 1 page leave me alone I'm lazy
  pmfree(getpaddr(vaddr), 1);
}