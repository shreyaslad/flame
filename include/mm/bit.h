/*
    bit.c
    Copyright Shreyas Lad (PenetratingShot) 2020

    Set and Clear Bit Operations
*/

#pragma once

#include <stdint.h>
#include <stddef.h>

#define middle_16(address) (((uint32_t)address) >> 16)
#define high_32(address) (((uint64_t)address) >> 32)

uint8_t getBit(uint8_t num, uint8_t bit);
void setBit(uint8_t* num, uint8_t bit, uint8_t state);

uint8_t getAbsoluteBit(uint64_t* bitmap, uint64_t bit);
void setAbsoluteBit(uint64_t* bitmap, uint64_t bit);
void clearAbsoluteBit(uint64_t* bitmap, uint64_t bit);