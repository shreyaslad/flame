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

  sprintf("Total Mem: %d\n\n", totalmem);

  uint64_t entries = totalmem / PAGESIZE / 8;
  memset(bitmap, 0, entries);

  uint64_t* bootPML4 = (uint64_t*)((uint64_t)getPML4() + HIGH_VMA);

  for (uint64_t i = 0; i < totalmem / PAGESIZE; i++) {
    vmap(i * PAGESIZE + HIGH_VMA, i * PAGESIZE, bootPML4, SUPERVISOR);
  }

  sprintd("Mapped All Memory");

  /*uint64_t kernelStartAddr = &_kernel_start;
  uint64_t kernelEndAddr = &_kernel_end;

  uint64_t kernelStartPhys = kernelStartAddr - KERNEL_HIGH_VMA;

  sprintf("%x - %x = %x\n", kernelStartAddr, KERNEL_HIGH_VMA, kernelStartPhys);
  uint64_t kernelPages = (kernelEndAddr - kernelStartAddr) / PAGESIZE + 1;
  sprintf("kernelStartAddr: %x | kernelEndAddr: %x | Pages: %d\n",
          kernelStartAddr, kernelEndAddr, kernelPages);

  for (uint64_t i = kernelStartAddr; i < kernelEndAddr + PAGESIZE;
       i += PAGESIZE) {
    vmap((uint64_t*)i, (uint64_t*)((uint64_t)i - KERNEL_HIGH_VMA), bootPML4,
         SUPERVISOR);
  }*/

  // this code subtracts things incorrectly
  // i cannot figure it out for the life of me
  // Just map everything in the boot tables

  /*uint64_t* newPML4Phys = pmalloc(1);
  uint64_t* newPML4 = (uint64_t*)((uint64_t)newPML4Phys + HIGH_VMA);
  sprintf("New PML4: %x\n", (uint64_t)newPML4);
  memset(newPML4, 0, 0x1000);

  for (uint64_t i = 0; i < totalmem / PAGESIZE; i++) {
    vmap(i * PAGESIZE + HIGH_VMA, i * PAGESIZE, newPML4, SUPERVISOR);
    sprintf("Pages Mapped: %d\n", i);
  }

  for (uint64_t i = kernelStartAddr; i < kernelEndAddr + PAGESIZE;
       i += PAGESIZE) {
    vmap((uint64_t*)i, (uint64_t*)((uint64_t)i - KERNEL_HIGH_VMA), newPML4,
         SUPERVISOR);
  }

  setPML4(newPML4Phys);*/

  currentAddr = mbd->mmap_addr;
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
  }
  sprintd("Marked All Memory");
}

void memset(void* bufptr, int value, size_t size) {
  unsigned char* buf = (unsigned char*)bufptr;
  for (uint64_t i = 0; i < size; i++) {
    buf[i] = value;
  }
}

void memcpy(void* dest, const void* src, size_t size) {
  const unsigned char* src2 = (const unsigned char*)src;
  unsigned char* dst = (unsigned char*)dest;
  for (uint64_t i = 0; i < size; i++) {
    dst[i] = src2[i];
  }
}

int memcmp(const void* s1, const void* s2, size_t n) {
  const uint8_t* p1 = s1;
  const uint8_t* p2 = s2;

  for (size_t i = 0; i < n; i++) {
    if (p1[i] != p2[i])
      return p1[i] < p2[i] ? -1 : 1;
  }

  return 0;
}

/* Allocation / Deallocation */
void* malloc(size_t bytes) {
  return (uint64_t)((uint64_t)kmalloc(bytes) + HIGH_VMA);
}

void free(void* vaddr) {
  liballoc_free(vaddr, 1); // frees 1 byte until i get around to not being lazy
}

void* realloc(void* ptr, size_t size) { krealloc(ptr, size); }