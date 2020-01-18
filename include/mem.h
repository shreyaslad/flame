#ifndef MEM_H
#define MEM_H

#include <stddef.h>
#include <function.h>
#include <drivers/screen.h>
#include "../cpu/type.h"

void memcpy(uint8_t* source, uint8_t* dest, uint32_t nbytes);
void memset(uint8_t* dest, uint8_t val, uint32_t len);

char memory[20000];

struct block {
	size_t size;
	int free;
	struct block *next;
};

void initialize();
void split(struct block* fitting_slot, size_t size);

void* malloc(size_t bytes);
void* realloc(void* pointer, size_t size);

void merge();
void free(void* ptr);

#endif
