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
  uint64_t totalBitsInBitmap = ((totalmem * 1000) / PAGESIZE) * 64;

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
  uint64_t firstBit = (uint64_t)ptr / PAGESIZE;
  uint64_t totalBitsInBitmap = ((totalmem * 1000) / PAGESIZE) * 64;

  for (uint64_t i = 0; i < totalBitsInBitmap; i++) {
    if (i == firstBit) {
      for (uint64_t j = 0; j < pages; j++) {
        setAbsoluteBitState(bitmap, j);
      }
      goto done;
    }
  }

done:
  return;
}

void* pmrealloc(void* ptr, size_t oldSize, size_t newSize) {
  uint64_t* newBuffer = (uint64_t*)pmalloc(newSize);
  memcpy(newBuffer, ptr, oldSize);
  pmfree(ptr, oldSize);
  return newBuffer;
}