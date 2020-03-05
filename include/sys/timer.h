#pragma once

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <sys/int/isr.h>
#include <sys/ports.h>
#include <proc/thread.h>
#include <mm/vmm.h>

extern uint64_t tick;
extern void _chgrip(uint64_t rip);

void init_timer(uint32_t freq);
void wait(uint32_t ticks);
void wait_s(uint32_t seconds);