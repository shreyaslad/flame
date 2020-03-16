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
  sprintf("Entries: %d\n", entries);
  memset(bitmap, 0, entries);

  uint64_t* bootPML4 = (uint64_t*)((uint64_t)getPML4() + HIGH_VMA);
  sprintf("PML4: %x\n", (uint64_t)bootPML4);

  sprintf("Pages: %d\n", totalmem / PAGESIZE);

  for (uint64_t i = 0; i < totalmem / PAGESIZE; i++) {
    vmap(i * PAGESIZE + HIGH_VMA, i * PAGESIZE, bootPML4, SUPERVISOR);
    sprintf("Pages Mapped: %d\n", i);
  }

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

  // setPML4(pml4ptr);
  // mapping for this table: 0000000008000000-0000000008200000 0000000000200000
  // vmap(pml4ptr, pml4ptr, (uint64_t*)getPML4(), SUPERVISOR);

  // map all of memory
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