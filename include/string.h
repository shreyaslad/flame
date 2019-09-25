/*
	string.h
	Copyright Shreyas Lad (PenetratingShot) 2019
	Licensed under the MIT License

	Handy string manipulation and conversion functions
*/

#pragma once

#include "../cpu/type.h"
#include "mem.h"
#include "null.h"

typedef char* String;

void int_to_ascii(int n, char str[]);
void hex_to_ascii(int n, char str[]);

void backspace(char s[]);

void reverse(char s[]);
int strlen(char s[]);
void append(char s[], char n);
char* strappend(char s[], char n[]);
char* strcpy(char* dest, char* src);

char* cut(String string, String deliminator);

int strcmp(char s1[], char s2[]);
int strcmpl(char s1[], char s2[], unsigned char x);
