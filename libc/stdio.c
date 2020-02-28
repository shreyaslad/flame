/*
    stdio.c
    Copyright Shreyas Lad (PenetratingShot) 2020

    Standard I/O Library
*/

#include <libc/stdio.h>

void printf(char* message, ...) {
  va_list ap;
  int len = strlen(message);

  char* buffer = (char*)malloc(sizeof(char));

  va_start(ap, message);

  for (int i = 0; i < len; i++) {
    if (message[i] == '%' && message[i + 1] == 's') {
      ++i;
      strcat(buffer, va_arg(ap, char*));
    } else if (message[i] == '%' && message[i + 1] == 'd') {
      ++i;
      char* buf = malloc(sizeof(char));
      itoa((int)va_arg(ap, int), buf);
      strcat(buffer, buf);

      buf[0] = '\0';
      free(buf);
    } else {
      append(buffer, message[i]);
    }
  }

  va_end(ap);

  kprint(buffer);
  free(buffer);
}

void sprintf(char* message, ...) {
  va_list ap;
  uint64_t len = strlen(message);

  char buffer[1024];

  va_start(ap, message);

  for (uint64_t i = 0; i < len; i++) {
    if (message[i] == '%' && message[i + 1] == 's') {
      ++i;
      strcat(buffer, va_arg(ap, char*));
    } else if (message[i] == '%' && message[i + 1] == 'd') {
      ++i;
      char buf[64];
      itoa((uint64_t)va_arg(ap, uint64_t), buf);
      strcat(buffer, buf);

      memset(buf, 0, 64);
    } else if (message[i] == '%' && message[i + 1] == 'x') {
      ++i;
      char buf[64];
      htoa((uint64_t)va_arg(ap, uint64_t), buf);

      strcat(buffer, buf);

      memset(buf, 0, 64);
    } else {
      append(buffer, message[i]);
    }
  }

  va_end(ap);

  sprint(buffer);
  memset(buffer, 0, 1024);
}