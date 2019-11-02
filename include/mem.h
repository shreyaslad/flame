#ifndef MEM_H
#define MEM_H

#include "../cpu/type.h"
#include <stddef.h>
#include "../drivers/screen.h"
#include <function.h>

void memory_copy(u8* source, u8* dest, int nbytes);
void memory_set(u8* dest, u8 val, u32 len);

// Deprecated kmalloc implementation
u32 kmalloc(u32 size, int align, u32* phys_addr);

char memory[20000];

struct block {
	size_t size;
	int free;
	struct block *next;
};

void initialize();
void split(struct block* fitting_slot, size_t size);

void *malloc(size_t bytes);
void* realloc(void* pointer, size_t size);

void merge();
void free(void* ptr);

#endif
