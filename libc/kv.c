/*
	kv.c
	Copyright Shreyas Lad (PenetratingShot) 2019
	Licensed under the MIT License

	Attempt at a Java hashmap-esque key-value data storage structure
*/

#include "kv.h"

static const size_t KVStoreSize = sizeof(KVStore);
static const size_t KVPairSize = sizeof(KVPair);

// Have to use int instead of bool to support 3 different error values instead of two
static int kvSort(const void* a, const void* b) {
	const KVPair* pairA = a;
	const KVPair* pairB = b;

	if (pairA->key > pairB->key) {
		return -1;
	}
	if (pairA->key < pairB->key) {
		return 1;
	}

	return 0;
}

static int kvSearch(const void* key, const void* elt) {
	const KVPair* pair = elt;

	if (key > pair->key) {
		return -1;
	}
	if (key < pair->key) {
		return 1;
	}

	return 0;
}

KVStore *kvsCreate(void) {
	KVStore* store = malloc(KVStoreSize);
	store->pairs = NULL;
	store->length = 0;
	
	return store;
}

static void kvCreatePair(KVStore* store, const void* key, void* value) {
	KVPair* pair;

	if (!store) return;
	++store->length;

	UNUSED(key);
	UNUSED(value);
}

static KVPair* kvGetPair(KVStore* store, const void* key) {
	if ((!store) || (!store->pairs)) {
		return NULL;
	}

	return bsearch(key, store->pairs, store->length, KVPairSize, kvSearch);
}

bool kvsDestroy(KVStore* store) {
	if (!store || store == NULL)
		return false;

	if (store->pairs) {
		free(store->pairs);
	}

	free(store);
	return true;
}