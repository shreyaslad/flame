/*
                mm.c
                Copyright Shreyas Lad (PenetratingShot) 2020

                Kernel Memory Manager
*/

#include <mm/mm.h>

/* Initialization */
void initMem(multiboot_info_t* mbd) {
  totalmem = (uint64_t)mbd->mem_upper;
  bitmapEntries = (uint64_t)(((totalmem * 1000) / PAGESIZE) /
                             8); // calculate the maximum amount of entries
                                 // possible in the bitmap to not overflow

  memset(bitmap, 0, (totalmem * 1000) / PAGESIZE / 8);
}

/* Allocation / Deallocation */
void* malloc(size_t bytes) {
  size_t pages = bytes / PAGESIZE;

  uint64_t* ret = (uint64_t*)pmalloc(pages);
  vmap(ret + KNL_HIGH_VMA, ret);
  ret += KNL_HIGH_VMA;

  return (void*)ret;
}

void free(void* vaddr) {
  vfree((uint64_t*)vaddr,
        PAGESIZE); // only frees 1 page leave me alone I'm lazy
  pmfree(getpaddr(vaddr), 1);
}