#include <mem.h>

uint64_t* bitmap = (uint64_t*)&__kernel_end;
uint64_t totalmem = 0;

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

/**********************
 * Private Memory API *
 **********************/

void initMem(multiboot_info_t* mbd) {
	totalmem = (uint64_t)mbd->mem_upper;

	memset(bitmap, 0, (totalmem * 1000) / 4096 / 8);
}

/* Virtual Memory Allocation */

/* Physical Memory Allocation */
size_t* palloc(size_t bytes) {
	uint64_t bitmapEntries = (totalmem * 1000) / 4096;

	// if there aren't enough bits left in a bitmap to designate for pages, just go to the next entry
	// im too lazy to write logic, it's literally a couple bits of space
	if (bytes > 4096) {
		uint64_t pagesToAllocate = (bytes / 4096) - 1;

		// allocate multiple bits
		// if not enough space, move to next bitmap entry
	} else {
		for (uint64_t i = 0; i < bitmapEntries; i++) {
			for (uint64_t j = 0; j < 64; j++) {
				uint64_t entry = bitmap[i];

				if (entry & ~0) {
					continue;
				} 
				
				if (rbit(entry, j) == 0) {
					sbit(bitmap, j);

					return (uint64_t *)(MEMBASE + (PAGESIZE * ((j / 8) * 8 + j)));
					break;
				}
			}
		}
	}

	return NULL;
}

void pfree(void* ptr) {
	uint64_t pagesToClear = ((uint64_t)ptr / 4096) - 1;

	for (uint64_t i = 0; i < pagesToClear; i++) {
		cbit(bitmap, i);
	}
}