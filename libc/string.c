#include "string.h"
#include "../cpu/type.h"
#include "bool.h"
#include "../libc/system/malloc.h"

void int_to_ascii(int n, char str[]) {
	int i, sign;
	if ((sign = n) < 0) n = -n;
	i = 0;
	do {
		str[i++] = n % 10 + '0';
	} while ((n /= 10) > 0);

	if (sign < 0) str[i++] = '-';
	str[i] = '\0';

	reverse(str);
}

void hex_to_ascii(int n, char str[]) {
	append(str, '0');
	append(str, 'x');
	char zeros = 0;

	s32 tmp;
	int i;
	for (i = 28; i > 0; i -= 4) {
		tmp = (n >> i) & 0xF;
		if (tmp == 0 && zeros == 0) continue;
		zeros = 1;
		if (tmp > 0xA) append(str, tmp - 0xA + 'a');
		else append(str, tmp + '0');
	}

	tmp = n & 0xF;
	if (tmp >= 0xA) append(str, tmp - 0xA + 'a');
	else append(str, tmp + '0');
}

void reverse(char s[]) {
	int c, i, j;
	for (i = 0, j = strlen(s) - 1; i < j; i++, j--) {
		c = s[i];
		s[i] = s[j];
		s[j] = c;
	}
}

int strlen(char s[]) {
	int i = 0;
	while (s[i] != '\0') ++i;
	return i;
}

void append(char s[], char n) {
	int len = strlen(s);
	s[len] = n;
	s[len + 1] = '\0';
}

char* strappend(char s[], char n[]) {
	int slen = strlen(s);
	int nlen = strlen(n);

	char buf[slen + nlen + 1]; //allocate a buffer that is the exact size of both char[], plus one byte for null termination
	
	for (int i = 0; i < nlen; i++) {
		for (int i = 0; i < slen; i++) {
			buf[i] = s[i]; //copy the first char[] to the buffer
		}

		buf[slen + i] = n[i]; //set everything after the first char[] offset to be the second char[]
	}

	buf[slen + nlen + 1] = '\0'; // null termination

	char* ret = malloc(sizeof(buf));
	return ret;
}

char* strcpy(char* dest, char* src) {
	char* saved = dest;
	while (*src) {
		*dest++ = *src++;
	}

	*dest = 0;
	return saved;
}

void backspace(char s[]) {
	int len = strlen(s);
	s[len - 1] = '\0';
}

bool strcmp(char s1[], char s2[]) {
	int i;
	for (i = 0; s1[i] == s2[i]; i++) {
		if (s1[i] == '\0') return 0;
	}
	//return s1[i] - s2[i];
	if ((s1[i] - s2[i]) == 0) {
		return true; // The two strings are the same
	}
	else {
		return false; // The two strings are not the same
	}
}

// Compare the first x number of characters between the two strings. Can be used for handling arguments
bool strcmpl(char s1[], char s2[], unsigned char x) {
	int i = 0;

	do {
		if (s1[i] == s2[i]) {
			return true;
		}
		else {
			return false;
		}

		i++;
	} while (i < x);

	return false;
}

int chartoint(char c) {
	return (int)c; // typecast char to int
}

int sizeofarr(char arr[]) {
	int total = 0;

	total = *(&arr + 1) - arr;
	return total;
}