/*
    bit.c
    Copyright Shreyas Lad (PenetratingShot) 2020

    Set and Clear Bit Operations
*/

#include <mm/bit.h>

uint8_t getBit(uint8_t num, uint8_t bit) {
    return ((num >> bit) & 1);
}

void setBit(uint8_t* num, uint8_t bit, uint8_t state) {
    if (state == 0) {
        *num &= ~(1 << bit);
    } else if (state == 1) {
        *num = (*num | (1 << bit));
    }
}

uint8_t getAbsoluteBitState(uint64_t* bitmap, uint64_t bit) {
    size_t off = bit / 64;
    size_t mask = (1 << (bit % 64));
    return (bitmap[off] & mask) == mask;
}

void setAbsoluteBitState(uint64_t* bitmap, uint64_t bit) {
    size_t off = bit / 64;
    size_t mask = (1 << (bit % 64));

    bitmap[off] |= mask;
}