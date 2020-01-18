#pragma once

#include <stdint.h>

#define middle_16(address) (((uint32_t)address) >> 16)
#define high_32(address) (((uint64_t)address) >> 32)

// remove this sometime
typedef unsigned int u32;
typedef int s32;
typedef unsigned short u16;
typedef short s16;
typedef unsigned char u8;
typedef char s8;