#pragma once

#include <string.h>
#include <sys/ports.h>
#include <sys/timer.h>

#define PORT 0x3f8 /* COM1 */

void init_serial();
void write_serial(char a);
void sprint(char *message);
void sprintd(char *message);
void sprint_uint(unsigned int num);
void sprint_int(int num);