/*
	kv.c
	Copyright Shreyas Lad (PenetratingShot) 2019

	Attempt at a Java hashmap-esque key-value data storage structure
*/

#include "kv.h"

bool setPair(struct HashMap map, String key, String value) {
	strcpy(map.Key, key);
	strcpy(map.Value, value);

	return false;
}

// map not needed since solely based on pointer
bool freePair(s16* pointer) {

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
