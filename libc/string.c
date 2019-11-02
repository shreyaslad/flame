/*
	string.c
	Copyright Shreyas Lad (PenetratingShot) 2019
	Licensed under the MIT License

	Handy string manipulation and conversion functions
*/

#include <string.h>

void itoa(int n, char str[]) {
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

char* strcat(char* dest, char* src) {
	char* ptr = dest + strlen(dest);

	while (*src != '\0') {
		*ptr++ = *src++;
	}

	*ptr = '\0';

	return dest;
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

int strcmp(char* s1, char* s2) {
	int i;
	for (i = 0; s1[i] == s2[i]; i++) {
		if (s1[i] == '\0') return 0;
	}

	return s1[i] - s2[i];
}

// Compare the first x number of characters between the two strings. Can be used for handling arguments
int strcmpl(char s1[], char s2[], unsigned char x) {
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

char* cut(String string, String deliminator) {
	static char* static_str = 0;
	int index, found = 0;
	
	int length = strlen(string);

	if (deliminator == 0 || (string == 0 && static_str == 0))
		return 0;

	if (string == 0)
		string = static_str;

	for (index = 0; index < length; index++)
		if (string[index] == deliminator[0]) {
			found = 1;
			break;
		}

	if (!found) {
		static_str = 0;
		return string;
	}

	if (string[0] == deliminator[0]) {
		static_str = (string + 1);
		return (char*)deliminator;
	}

	string[index] = '\0';

	if ((string + index + 1) != 0)
		static_str = (string + index + 1);
	else
		static_str = 0;

	return string;
}