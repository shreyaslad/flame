/*
  pmm.c
  Copyright Shreyas Lad (PenetratingShot) 2020

  Physical Memory Manager
*/

#include <mm/pmm.h>

uint64_t* bitmap = (uint64_t*)&__kernel_end;

uint64_t totalmem;
uint64_t bitmapEntries;

/*******************
 * Private PMM API *
 *******************/

void* pmalloc(size_t pages) {
  uint64_t firstBit = 0;
  uint64_t concurrentBits = 0;

  uint64_t totalBitsInBitmap = pages * 64;

  for (uint64_t i = 0; i < totalBitsInBitmap; i++) {
    if (getAbsoluteBitState(bitmap, i) == 0) {
      if (concurrentBits == 0) {
        firstBit = i;
      }

      concurrentBits++;

      if (pages == concurrentBits) {
        goto alloc;
      }
    } else {
      firstBit = 0;
      concurrentBits = 0;

      continue;
    }
  }

  return NULL;

alloc:
  // iterate over bits now that a block has been found
  for (uint64_t i = firstBit; i < firstBit + pages; i++) {
    setAbsoluteBitState(bitmap, i);
  }

  return (void*)(firstBit * PAGESIZE);
}

void pmfree(void* ptr, size_t pages) {
  uint64_t absoluteStartBit = (uint64_t)ptr / PAGESIZE;
  for (uint64_t i = absoluteStartBit; i < pages; i++) {
    setAbsoluteBitState(bitmap, i);
  }
}