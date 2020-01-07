#pragma once

#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <function.h>
#include <drivers/time.h>
#include "type.h"
#include "isr.h"
#include "ports.h"

void init_timer(uint32_t freq);
void wait(uint32_t seconds);