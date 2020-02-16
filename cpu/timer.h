#pragma once

#include "isr.h"
#include "ports.h"
#include "type.h"
#include <drivers/time.h>
#include <function.h>
#include <stdbool.h>
#include <stdint.h>
#include <string.h>

extern uint32_t tick;

void init_timer(uint32_t freq);
void wait(uint32_t ticks);
void wait_s(uint32_t seconds);