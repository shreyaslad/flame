/*
    stdio.h
    Copyright Shreyas Lad (PenetratingShot) 2020

    Standard I/O Library
*/

#pragma once

#include <stdint.h>
#include <stddef.h>
#include <stdarg.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <devices/screen/screen.h>
#include <devices/screen/serial.h>

#define READ 0
#define WRITE 1
#define RANDW 2

void printf(char* message, ...);
void sprintf(char* message, ...);

char* fread(uint64_t fd, uint64_t bytes);