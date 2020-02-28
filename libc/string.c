/*
                                string.c
                                Copyright Shreyas Lad (PenetratingShot) 2019
                                Licensed under the MIT License

                                Handy string manipulation and conversion
   functions
*/

#include <string.h>

#include <stdint.h>

void itoa(int n, char str[]) {
  int i, sign;
  if ((sign = n) < 0)
    n = -n;
  i = 0;
  do {
    str[i++] = n % 10 + '0';
  } while ((n /= 10) > 0);

  if (sign < 0)
    str[i++] = '-';
  str[i] = '\0';

  reverse(str);
}

void itoa_uint8(uint8_t n, char str[]) {
  uint8_t i = 0;
  do {
    str[i++] = n % 10 + '0';
  } while ((n /= 10) > 0);

  str[i] = '\0';

  reverse(str);
}

void htoa(uint64_t in, char* str) {
  uint32_t pos = 0;
  uint8_t tmp;

  str[pos++] = '0';
  str[pos++] = 'x';

  for (uint16_t i = 60; i > 0; i -= 4) {
    tmp = (uint8_t)((in >> i) & 0xf);
    if (tmp >= 0xa) {
      str[pos++] = (tmp - 0xa) + 'A';
    } else {
      str[pos++] = tmp + '0';
    }
  }

  tmp = (uint8_t)(in & 0xf);
  if (tmp >= 0xa) {
    str[pos++] = (tmp - 0xa) + 'A';
  } else {
    str[pos++] = tmp + '0';
  }

  str[pos] = '\0'; // nullify
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
  while (s[i] != '\0')
    ++i;
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
    if (s1[i] == '\0')
      return 0;
  }

  return s1[i] - s2[i];
}

// Compare the first x number of characters between the two strings. Can be used
// for handling arguments
int strcmpl(char s1[], char s2[], unsigned char x) {
  int i = 0;

  do {
    if (s1[i] == s2[i]) {
      return true;
    } else {
      return false;
    }

    i++;
  } while (i < x);

  return false;
}

char* strtok(char* str, const char* delimiter) {
  static char* buffer;

  if (str != NULL) {
    buffer = str;
  }

  if (buffer[0] == '\0') {
    return NULL;
  }

  char* ret = buffer;

  for (char* b = buffer; *b != '\0'; b++) {
    for (const char* d = delimiter; *d != '\0'; d++) {
      if (*b == *d) {
        *b = '\0';
        buffer = b + 1;

        // Skip the beginning delimiters
        if (b == ret) {
          ret++;
          continue;
        }

        return ret;
      }
    }
  }

  return ret;
}

uint64_t strnchr(char* str, char s) {
  uint64_t count = 0;

  for (uint64_t i = 0; i < strlen(str); i++) {
    if (strcmp(str[i], s) == 0) {
      count++;
    }
  }

  return count;
}

char removeNull(char* s) {
  char ret = s[0];
  return ret;
}

void remove(char* s, uint32_t pos) {
  int len = strlen(s);
  char prev = removeNull("\0");
  char tempE;
  for (uint32_t x = len; x > pos - 2; x--) {
    tempE = s[x];
    s[x] = prev;
    prev = tempE;
  }
  s[len - 1] = '\0';
}