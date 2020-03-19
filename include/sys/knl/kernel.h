/*
    kernel.h
    Copyright Shreyas Lad (PenetratingShot) 2020

    Kernel
*/

#pragma once

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <boot/multiboot.h>
#include <drivers/screen/screen.h>
#include <drivers/screen/vga.h>
#include <drivers/acpi/acpi.h>
#include <lai/helpers/pm.h>
#include <lai/helpers/sci.h>
#include <time.h>

extern void _initUserspace();