/*
    stdio.c
    Copyright Shreyas Lad (PenetratingShot) 2020

    Standard I/O Library
*/

#include <libc/stdio.h>

void printf(char* message, ...) {
  va_list ap;
  uint64_t len = strlen(message);

  char buffer[1024];
  memset(buffer, NULL, 1024);

  va_start(ap, message);

  for (uint64_t i = 0; i < len; i++) {
    if (message[i] == '%' && message[i + 1] == 's') {
      ++i;
      strcat(buffer, va_arg(ap, char*));
    } else if (message[i] == '%' && message[i + 1] == 'd') {
      ++i;
      char buf[64];
      memset(buf, NULL, 64);
      itoa((uint64_t)va_arg(ap, uint64_t), buf);
      strcat(buffer, buf);

      memset(buf, NULL, 64);
    } else if (message[i] == '%' && message[i + 1] == 'x') {
      ++i;
      char buf[64];
      memset(buf, NULL, 64);
      htoa((uint64_t)va_arg(ap, uint64_t), buf);

      strcat(buffer, buf);

      memset(buf, NULL, 64);
    } else {
      append(buffer, message[i]);
    }
  }

  va_end(ap);

  kprint(buffer);
}

void sprintf(char* message, ...) {
  va_list ap;
  uint64_t len = strlen(message);

  char buffer[1024];
  memset(buffer, NULL, 1024);

  va_start(ap, message);

  for (uint64_t i = 0; i < len; i++) {
    if (message[i] == '%' && message[i + 1] == 's') {
      ++i;
      strcat(buffer, va_arg(ap, char*));
    } else if (message[i] == '%' && message[i + 1] == 'd') {
      ++i;
      char buf[64];
      memset(buf, NULL, 64);
      itoa((uint64_t)va_arg(ap, uint64_t), buf);
      strcat(buffer, buf);

      memset(buf, NULL, 64);
    } else if (message[i] == '%' && message[i + 1] == 'x') {
      ++i;
      char buf[64];
      memset(buf, NULL, 64);
      htoa((uint64_t)va_arg(ap, uint64_t), buf);

      strcat(buffer, buf);

      memset(buf, NULL, 64);
    } else {
      append(buffer, message[i]);
    }
  }

  va_end(ap);

  sprint(buffer);
}

uint64_t open(char* filename, uint64_t action) {
  // check if the action the user wants to perform is allowed by the node
  ; // find the struct by parsing the filename
  return 0;

  UNSUED(filename);
  UNUSED(action);
}

char* fread(uint64_t fd, uint64_t bytes) {
  asm("syscall" ::"a"(1), "b"(fd), "c"(bytes));
  return NULL;

  UNUSED(fd);
  UNUSED(bytes);
}