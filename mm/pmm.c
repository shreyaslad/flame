/*
    pmm.c
    Copyright Shreyas Lad (PenetratingShot) 2020

    Physical Memory Manager
*/

#include <mm/pmm.h>

uint64_t totalmem = 0;
uint64_t bitmapEntries = 0;

/*********************
 * Public Memory API *
 *********************/
void memcpy(uint8_t* source, uint8_t* dest, uint32_t nbytes) {
	for (uint32_t i = 0; i < nbytes; i++) {
		*(dest + i) = *(source + i);
	}
}

void memset(uint64_t* dest, uint64_t val, uint64_t len) {
	uint64_t* temp = (uint64_t*)dest;
	for (; len != 0; len--) * temp++ = val;
}

/*******************
 * Private PMM API *
 *******************/

/* Physical Memory Allocation */
void* palloc(uint64_t bytes) {
  /*
    1. Find the amount of pages needed
    2. Find how many entries are needed in the bitmap to not overflow and write to all mem past &__kernel_end
    3. Find open bits in the bitmap
    4. If no contiguous bits left, write whatever bits are left to the bitmap and add the rest to a new entry
  */

  uint64_t pagesToAllocate = bytes / PAGESIZE; // value should be floored
  
}

void pfree(void* ptr) {
	uint64_t pagesToClear = ((uint64_t)ptr / PAGESIZE) - 1;

	for (uint64_t i = 0; i < pagesToClear; i++) {
		cbit(bitmap, i);
	}
}