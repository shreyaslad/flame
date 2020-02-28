#pragma once

#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <sys/int/isr.h>
#include <sys/ports.h>
#include <devices/time.h>

extern uint32_t tick;

void init_timer(uint32_t freq);
void wait(uint32_t ticks);
void wait_s(uint32_t seconds);