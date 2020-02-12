/*
    bit.c
    Copyright Shreyas Lad (PenetratingShot) 2020

    Bit r/w operations
*/

#include <bit.h>

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