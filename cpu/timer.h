#ifndef TIMER_H
#define TIMER_H

#include "type.h"
#include "isr.h"
#include "ports.h"
#include <function.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>

void init_timer(uint32_t freq);

#endif
