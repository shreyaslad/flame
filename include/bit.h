/*
    bit.h
    Copyright Shreyas Lad (PenetratingShot) 2020

    Bit r/w operations
*/

#pragma once

#include <stdint.h>

uint8_t getBit(uint8_t num, uint8_t bit);
void setBit(uint8_t* num, uint8_t bit, uint8_t state);