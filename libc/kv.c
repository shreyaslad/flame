/*
	kv.c
	Copyright Shreyas Lad (PenetratingShot) 2019

	Attempt at a Java hashmap-esque key-value data storage structure
*/

#include "kv.h"

bool setPair(struct HashMap map, String key, String value) {
	s8* pointer;

	pointer = malloc(sizeof(key) + 1 + sizeof(value)); // reserve the exact amount of space for key=value

	strcpy(map.Key, key);
	strcpy(map.Value, value);
	strcpy(map.dataPointer, pointer); // store pointer to data in struct (struct is a shell for the data inside the same scope it was declared in)

	// check if pointer is valid or not. If not, something went wrong and return false
	if (pointer == NULL) {
		return false;
	}

	return true;
}

// map not needed since solely based on pointer
bool freePairPointer(s8* pointer) {

	free((void *) pointer);

	return false;
}

bool freePairMap(struct HashMap map) {

	if (map.dataPointer == NULL) {
		return false; // Something went wrong and the pointer to the data wasn't set
	}

	free((void*)map.dataPointer);

	return true;
}

String getKey(struct HashMap map) {

	UNUSED(map);
	return NULL;
}

String getValue(struct HashMap map) {

	UNUSED(map);
	return NULL;
}
