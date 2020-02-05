/*
    bitmap.c
    Copyright Shreyas Lad (PenetratingShot) 2020

    Interations with the bitmap
*/

#include <mm/bitmap.h>

uint64_t totalmem = 0;
uint64_t bitmapEntries = 0;

void initMem(multiboot_info_t* mbd) {
	totalmem = (uint64_t)mbd->mem_upper;
	bitmapEntries = (uint64_t)(((totalmem * 1000) / PAGESIZE) / 8); // calculate the maximum amount of entries possible in the bitmap to not overflow

	memset(bitmap, 0, (totalmem * 1000) / PAGESIZE / 8);
}

index_t* getFreeIndicies(uint64_t pages) {
    index_t* index;
    index->entries = bitmapEntries; // bruh
    index->row = 0;
    index->bit = 0;

    uint64_t freebits = 0;
    uint64_t firstaddr = 0;

    for (int i = 0; i < bitmapEntries; i++) {
        uint64_t entry = bitmap[i];

        for (int j = 0; j < 63; j++) {
            if (rbit(entry, j) == 0) {
                if (freebits == 0) {
                    firstaddr = j;
                }

                freebits++;

                if (freebits == pages) {
                    index->row = entry;
                    index->bit = firstaddr;

                    goto done;
                }

                if (j == 63 && rbit(entry + 1, 0) == 1) {
                    return NULL; // block is not contiguous
                } else {
                    break; // move to the next entry
                }
            } else {
                freebits = 0;
                firstaddr = 0;
            }
        }
    }

    return NULL;

    done:
        return index;
}