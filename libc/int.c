/*
	int.c
	Copyright Shreyas Lad (PenetratingShot) 2019
	Licensed under the MIT License

	Contains functions for handy integer conversions
*/

#include <int.h>

int octalToDecimal(int octal) {
	int decimal, i = 0;
	
	while (octal != 0) {
		decimal += (octal % 10) * power(8, i);
		++i;
		octal /= 10;
	}

	i = 1;
	
	return decimal;
}

int decimalToOctal(int decimal) {
	int octal = 0;
	int i = 1;

	while (decimal != 0) {
		octal += (decimal % 8) * i;
		decimal /= 8;
		i *= 10;
	}

	return octal;
}

int power(int base, int power) {
	int result = 1;

	for (int i = 0; i < power; i++) {
		result *= base;
	}

	return result;
}
