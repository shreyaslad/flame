#pragma once

void int_to_ascii(int n, char str[]);
void hex_to_ascii(int n, char str[]);

void backspace(char s[]);

void reverse(char s[]);
int strlen(char s[]);
void append(char s[], char n);
char* strappend(char s[], char n[]);
char* strcpy(char* dest, char* src);
int strcmp(char s1[], char s2[]);
int strcmpl(char s1[], char s2[], unsigned char x);
int chartoint(char c);

int sizeofarr(char arr[]);