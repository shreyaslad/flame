/*
	int.h
	Copyright Shreyas Lad (PenetratingShot) 2019
	Licensed under the MIT License

	Contains functions for handy integer conversions
*/

#pragma once

#include <stdint.h>

int octalToDecimal(int octal);
int decimalToOctal(int decimal);

int power(int base, int exponent);
uint32_t abs(int num);