/*
	kv.h
	Copyright Shreyas Lad (PenetratingShot) 2019

	Attempt at a Java hashmap-esque key-value data storage structure
*/

#pragma once

#include "../cpu/type.h"
#include "string.h"
#include "bool.h"
#include "null.h"
#include "function.h"
#include "mem.h"

// Use malloc to reserve concatenated string of key=value
// dataPointer holds pointer to this data
// When get function is called, it grabs the data at this pointer
// Can get the key or the value depending on the index of the array when you do split('=')
// Can delete the pair by calling free on the stored data pointer
struct HashMap {
	String Key;
	String Value;
	s8* dataPointer;
} HashMap;

bool setPair(struct HashMap map, String key, String value); // Makes calls to malloc and sets struct values
bool freePair(s8* pointer); // Calls free on the pointer

String getKey(struct HashMap map); // map.dataPointer
String getValue(struct HashMap map);