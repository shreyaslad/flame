/*
    pmm.c
    Copyright Shreyas Lad (PenetratingShot) 2020

    Physical Memory Manager
*/

#include <mm/pmm.h>

uint64_t* bitmap = (uint64_t *)&__kernel_end;

uint64_t totalmem;
uint64_t bitmapEntries;

/*********************
 * Public Memory API *
 *********************/
void memcpy(uint8_t* source, uint8_t* dest, uint32_t nbytes) {
	for (uint32_t i = 0; i < nbytes; i++) {
		*(dest + i) = *(source + i);
	}
}

void memset(void* dest, int val, size_t len) {
    for (uint8_t *temp = dest; len--;)
        *temp++ = val;
}

/*******************
 * Private PMM API *
 *******************/

void* pmalloc(size_t bytes) {
  uint64_t pages = bytes / PAGESIZE;

  uint64_t firstBit = 0;
  uint64_t concurrentBits = 0;
  uint64_t bitsToAlloc = pages + 1;

  uint64_t totalBitsInBitmap = pages * 64;

  for (uint64_t i = 0; i < totalBitsInBitmap; i++) {
    if (getAbsoluteBitState(bitmap, i) == 0) {
      if (concurrentBits == 0) {
        firstBit = i;
      }

      concurrentBits++;

      if (bitsToAlloc == concurrentBits) {
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
    for (uint64_t i = firstBit; i < bitsToAlloc; i++) {
      setAbsoluteBitState(bitmap, i);
    }

    return (void*)(firstBit * PAGESIZE);
  
}