/*
	bsearch.h
	Copyright Shreyas Lad (PenetratingShot) 2019
	Licensed under the MIT License

	Header file for binary search for an array of elements
	Originals: 
		| torvalds/linux/lib/bsearch.h
		| torvalds/linux/lib/bsearch.c
*/

#pragma once

#include "../cpu/type.h"
#include "null.h"

void* bsearch(const void* key, const void* base, size_t num, size_t size, int(*compare)(const void* key, const void* elt));
void* qsort(const void* base, size_t num, size_t size, int(*sort)(const void* key, const void* elt));