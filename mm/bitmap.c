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
	bitmapEntries = (uint64_t)((totalmem / PAGESIZE) / 64); // calculate the maximum amount of entries possible in the bitmap to not overflow

	memset(bitmap, 0, (totalmem * 1000) / PAGESIZE / 8);
}

struct index_t* getFreeIndicies(uint64_t indicies) {
    /*
    1. 
    */

    struct index_t* bitmapIndex;
    bitmapIndex->index = 0;
    bitmapIndex->travel = 0;

    uint64_t readBits = 0;
    uint64_t freeIndicies = 0;

    for (int i = 0; i < bitmapEntries; i++) {
        readBits = 0;
        for (int j = 0; j < 64; j++) {
            uint64_t entry = bitmap[i];
            readBits++;
            
            if (rbit(entry, j) == 1 && readBits == 63) break; // if nothing is available

            // do things
        }
    }
}