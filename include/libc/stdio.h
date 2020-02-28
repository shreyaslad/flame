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
#include <devices/screen/screen.h>
#include <devices/screen/serial.h>
#include <mm/pmm.h>

void printf(char* message, ...);
void sprintf(char* message, ...);