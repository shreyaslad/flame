/*
                                string.h
                                Copyright Shreyas Lad (PenetratingShot) 2019
                                Licensed under the MIT License

                                Handy string manipulation and conversion
   functions
*/

#pragma once

#include "../cpu/type.h"
#include <mm/mm.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

void itoa(int n, char str[]);
void itoa_uint8(uint8_t num, char str[]);
void hex_to_ascii(int n, char str[]);

void backspace(char s[]);

void reverse(char s[]);
int strlen(char s[]);
void append(char s[], char n);
char* strcat(char* s, char* n);
char* strcpy(char* dest, char* src);

int strcmp(char* s1, char* s);
int strcmpl(char s1[], char s2[], unsigned char x);

char removeNull(char* s);
void remove(char* s, uint32_t pos);