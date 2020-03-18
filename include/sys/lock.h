/*
    lock.h
    Copyright Shreyas Lad (PenetratingShot) 2020

    Locks
*/

#pragma once

#include <stdint.h>

typedef volatile uint64_t spinlock_t;

extern void spinlock_lock(spinlock_t* spinlock);
extern void spinlock_unlock(spinlock_t* spinlock);