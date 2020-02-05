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

// loop through each entry
// check if bit location + amount of pages requested are all 0
// if yes, return address of initial bit location
// if no, keep trying until end of bitmap is reached
// 
index_t* getFreeIndicies(uint64_t pages) {
    index_t* index;
    index->row = 0;
    index->bit = 0;

    uint64_t freebits = 0; // free bits per entry

    for (int i = 0; i < bitmapEntries; i++) {
        uint64_t entry = bitmap[i];

        for (int j = 0; j < 64; j++) {
            if ((0xFFFFFFFF >> pages) & entry == 0) {
                index->row = entry;
                index->bit = j;

                goto done;
            }

            if (rbit(entry, j) == ~0) {
                break;
            } else {
                // allocate the last bits of this page and determine if the next page has any free bits at the beginning
            }
        }
    }

    return NULL;

    done:
        return index;
}