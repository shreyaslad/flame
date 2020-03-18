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
#include <mm/vmm.h>
#include <drivers/screen/screen.h>
#include <drivers/screen/vga.h>
#include <drivers/acpi/acpi.h>
#include <time.h>
#include <fs/vfs.h>
#include <fs/fd.h>
#include <proc/thread.h>
#include <proc/syscall.h>

extern void _initUserspace();