#ifndef MALLOC_H
#define MALLOC_H

#include "../../cpu/type.h"

// Size of each memory block is 20k bytes
char memory[20000];

struct block {
	size_t size;
	int free;
	struct block *next;
};

struct block* freeList = (void*)memory;

void initialize();
void split(struct block* fitting_slot, size_t size);
void *malloc(size_t bytes);
void merge();
void free(void* ptr);

void memory_copy(u8* source, u8* dest, int nbytes);
void memory_set(u8* dest, u8 val, u32 len);

#endif