/*
    mm.c
    Copyright Shreyas Lad (PenetratingShot) 2020

    Kernel Memory Manager
*/

#include <mm/mm.h>

void* malloc(size_t bytes) {
    uint64_t* ret = pmalloc(bytes);
    vmap(ret + KNL_HIGH_VMA, ret);
    ret += KNL_HIGH_VMA;

    return (void*)ret;
}

void free(uint64_t* vaddr) {
    UNUSED(vaddr);
}