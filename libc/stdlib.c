/*
  stdlib.c
  Copyright Shreyas Lad (PenetratingShot) 2020

  Standard Library
*/

#include <libc/stdlib.h>

/* Initialization */
void initMem(multiboot_info_t* mbd) {
  totalmem = (uint64_t)mbd->mem_upper;
  bitmapEntries = (uint64_t)(((totalmem * 1000) / PAGESIZE) /
                             8); // calculate the maximum amount of entries
                                 // possible in the bitmap to not overflow

  memset(bitmap, 0, (totalmem * 1000) / PAGESIZE / 8);
}

void memcpy(uint8_t* source, uint8_t* dest, uint32_t nbytes) {
  for (uint32_t i = 0; i < nbytes; i++) {
    *(dest + i) = *(source + i);
  }
}

void memset(void* dest, int val, size_t len) {
  for (uint8_t* temp = dest; len--;)
    *temp++ = val;
}

/* Allocation / Deallocation */
void* malloc(size_t bytes) {
  uint64_t pages = bytes / PAGESIZE;

  // stupid hack
  if (bytes > 0 && pages == 0) {
    pages++;
  }

  uint64_t* ret = (uint64_t*)pmalloc(pages);

  vmap(ret + 0xFFFF800000000000, ret, pages);
  ret = (uint64_t)ret + 0xFFFF800000000000;

  return ret;
}

void free(void* vaddr) {
  vfree((uint64_t*)vaddr,
        PAGESIZE); // only frees 1 page leave me alone I'm lazy
  pmfree(getpaddr(vaddr), 1);
}