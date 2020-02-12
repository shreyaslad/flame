/*
    mm.h
    Copyright Shreyas Lad (PenetratingShot) 2020

    Kernel Memory Manager
*/

#pragma once

#include <mm/pmm.h>
#include <mm/vmm.h>

extern KNL_HIGH_VMA;

void* malloc(size_t bytes);