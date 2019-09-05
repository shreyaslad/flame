#pragma once

#include "../../cpu/type.h"

// Size of each memory block is 20k bytes
char memory[20000];

struct block {
	size_t size;
	int free;
	struct block *next;
};

void initialize();
void split(struct block* fitting_slot, size_t size);
void *malloc(size_t bytes);
void merge();
void free(void* ptr);
