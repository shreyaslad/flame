#include <mem.h>

#include "freeList.h"

void memory_copy(u8* source, u8* dest, int nbytes) {
	int i;
	for (i = 0; i < nbytes; i++) {
		*(dest + i) = *(source + i);
	}
}

void memory_set(u8 * dest, u8 val, u32 len) {
	u8* temp = (u8*)dest;
	for (; len != 0; len--) * temp++ = val;
}

// kernel starts at 0x01000
u32 free_mem_addr = 0x10000;
/* Implementation is just a pointer to some free memory which
 * keeps growing */
u32 kmalloc(u32 size, int align, u32 * phys_addr) {
	/* Pages are aligned to 4K, or 0x1000 */
	if (align == 1 && (free_mem_addr & 0xFFFFF000)) {
		free_mem_addr &= 0xFFFFF000;
		free_mem_addr += 0x1000;
	}
	
	if (phys_addr)* phys_addr = free_mem_addr;

	u32 ret = free_mem_addr;
	free_mem_addr += size; 
	return ret;
}

void initialize() {
	freeList->size = 20000 - sizeof(struct block);
	freeList->free = 1;
	freeList->next = 0;
}

void split(struct block* fitting_slot, size_t size) {
	struct block* new = (void*)(fitting_slot + size + sizeof(struct block));
	new->size = (fitting_slot->size) - size - sizeof(struct block);
	new->free = 1;
	new->next = fitting_slot->next;
	fitting_slot->size = size;
	fitting_slot->free = 0;
	fitting_slot->next = new;
}

void* malloc(size_t bytes) {

	struct block* curr, * prev;
	void* result;

	if (!(freeList->size)) {
		initialize();
	}

	curr = freeList;
	while ((((curr->size) < bytes) || ((curr->free) == 0)) && (curr->next != 0)) {
		prev = curr;
		curr = curr->next;
	}

	if ((curr->size) == bytes) {
		curr->free = 0;
		result = (void*)(++curr);

		return result;
	}
	else if ((curr->size) > (bytes + sizeof(struct block))) {
		split(curr, bytes);
		result = (void*)(++curr);
		return result;
	}
	else {
		result = 0;
		kprint("Not enough memory left.\nPlease free some up before attempting to allocate more.\n");
		return result;
	}

	UNUSED(*prev);
}

void* realloc(void* pointer, size_t size) {

	UNUSED(pointer);
	UNUSED(size);
	return NULL;
}

void merge() {
	struct block* curr, * prev;
	curr = freeList;
	while ((curr->next) != 0) {
		if ((curr->free) && (curr->next->free)) {
			curr->size += (curr->next->size) + sizeof(struct block);
			curr->next = curr->next->next;
		}
		prev = curr;
		curr = curr->next;
	}

	UNUSED(*prev);
}

void free(void* ptr) {
	if (((void*)memory <= ptr) && (ptr <= (void*)(memory + 20000))) {
		struct block* curr = ptr;
		--curr;

		curr->free = 1;

		merge();
	}
	else {
		kprint("Provide a valid pointer allocated by malloc!\n");
	}
}
