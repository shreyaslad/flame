/*
    kernel.h
    Copyright Shreyas Lad (PenetratingShot) 2020

    Kernel
*/

#pragma once

#include <stdint.h>
#include <stdio.h>
#include <boot/multiboot.h>
#include <mm/pmm.h>
#include <devices/screen/screen.h>
#include <time.h>
#include <fs/vfs.h>
#include <fs/fd.h>
#include <proc/thread.h>
#include <proc/syscall.h>

extern void _initUserspace();