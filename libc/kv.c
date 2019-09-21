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

	return false;
}

// map not needed since solely based on pointer
bool freePair(s8* pointer) {

	UNUSED(pointer);
	return false;
}

String getKey(struct HashMap map) {

	UNUSED(map);
	return NULL;
}

String getValue(struct HashMap map) {

	UNUSED(map);
	return NULL;
}
