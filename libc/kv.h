/*
	kv.h
	Copyright Shreyas Lad (PenetratingShot) 2019
	Licensed under the MIT License

	Attempt at a Java hashmap-esque key-value data storage structure
*/

#pragma once

#include "../cpu/type.h"
#include "string.h"
#include "null.h"
#include "function.h"
#include "mem.h"
#include "search.h"

// key and value can be of any type
typedef struct {
	const void* key;
	void* value;
} KVPair;

typedef struct {
	KVPair* pairs;
	u32 length;
} KVStore;

static int kvSort(const void* a, const void* b);
static int kvSearch(const void* key, const void* elt);

KVStore *kvsCreate(void);
static void kvCreatePair(KVStore* store, const void* key, void* value);

static KVPair* kvGetPair(KVStore* store, const void* key);

bool kvsDestroy(KVStore* store);
