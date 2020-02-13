/*
    bit.c
    Copyright Shreyas Lad (PenetratingShot) 2020

    Set and Clear Bit Operations
*/

#pragma once

#include <stdint.h>
#include <stddef.h>

uint8_t getBit(uint8_t num, uint8_t bit);
void setBit(uint8_t* num, uint8_t bit, uint8_t state);