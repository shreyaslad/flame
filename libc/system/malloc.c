#include "../../cpu/type.h"
#include "malloc.h"
#include "../../drivers/screen.h"
#include "../../libc/function.h"

void initialize() {
	freeList->size = 20000 - sizeof(struct block);
	freeList->free = 1;
	freeList->next = 0;
}

void split(struct block* fitting_slot, size_t size) {
	struct block* new = (void*)((void*)fitting_slot + size + sizeof(struct block));
	new->size = (fitting_slot->size) - size - sizeof(struct block);
	new->free = 1;
	new->next = fitting_slot->next;
	fitting_slot->size = size;
	fitting_slot->free = 0;
	fitting_slot->next = new;
}

void *malloc(size_t bytes) {

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

	UNUSED(free);
	UNUSED(*prev); // yes, this is necessary
}