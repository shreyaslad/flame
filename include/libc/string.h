/*
   string.h
   Copyright Shreyas Lad (PenetratingShot) 2019
   Licensed under the MIT License

   Handy string manipulation and conversion
   functions
*/

#pragma once

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>

void itoa(int n, char str[]);
void itoa_uint8(uint8_t num, char str[]);
void htoa(uint64_t num, char* str);

void backspace(char s[]);

void reverse(char s[]);
int strlen(char s[]);
void append(char s[], char n);
char* strcat(char* s, char* n);
char* strcpy(char* dest, char* src);

int strcmp(char* s1, char* s);
int strcmpl(char s1[], char s2[], unsigned char x);
char* strtok(char* str, const char* delimiter);
uint64_t strnchr(char* str, char s);

char removeNull(char* s);
void remove(char* s, uint32_t pos);