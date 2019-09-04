#ifndef TIMER_H
#define TIMER_H

#include "type.h"
#include "../libc/bool.h"

void init_timer(u32 freq);

void wait(u32 ticks, bool flag);

#endif
