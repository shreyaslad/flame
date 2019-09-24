#ifndef TIMER_H
#define TIMER_H

#include "type.h"
#include "isr.h"
#include "ports.h"
#include "../libc/function.h"

void init_timer(u32 freq);

void wait(u32 ticks, bool flag);

#endif
