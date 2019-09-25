/*
	bsearch.c
	Copyright Shreyas Lad (PenetratingShot) 2019
	Licensed under the MIT License

	Header file for binary search for an array of elements
	Originals:
		| torvalds/linux/lib/bsearch.h
		| torvalds/linux/lib/bsearch.c
*/

#include "search.h"

void* bsearch(const void* key, const void* base, size_t num, size_t size, int(*compare)(const void* key, const void* elt)) {
	const char* pivot;
	int result;

	while (num > 0) {
		pivot = base + (num >> 1) * size;
		result = compare(key, pivot);

		if (result == 0) return (void*)pivot;

		if (result > 0) {
			base = pivot + size;
			num--;
		}

		num >>= 1;
	}
	
	return NULL;
}

void* qsort(const void* base, size_t num, size_t size, int(*sort)(const void* key, const void* elt)) {
	const char* pivot;
	int result;

	while (num > 0) {
		pivot = base + (num >> 1) * size;
		result = sort(base, pivot);

		if (result == 0) return (void*)pivot;

		if (result > 0) {
			base = pivot + size;
			num--;
		}

		num >>= 1;
	}

	return NULL;
}