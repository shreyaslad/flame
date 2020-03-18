/*
    mem.h
    Copyright Shreyas Lad (PenetratingShot) 2020

    Memory Definitions
*/

#pragma once

#define KERNEL_HIGH_VMA 0xFFFFFFFF80000000
#define HIGH_VMA 0xFFFF800000000000

#define TABLESIZE 0x1000
#define PAGESIZE 0x200000

#define RMFLAGS 0x000FFFFFFFFFF000
#define TABLEPRESENT (1 << 0)
#define TABLEWRITE (1 << 1)
#define TABLEUSER (1 << 2)
#define TABLEHUGE (1 << 7)

#define SUPERVISOR 0
#define USER 1