#pragma once

#include "../cpu/type.h"
#include "../cpu/ports.h"
#include "../cpu/isr.h"
#include "screen.h"
#include <string.h>
#include <function.h>
#include <stdint.h>
#include <stdbool.h>
#include "../kernel/kernel.h"

extern bool canType;

void logic(uint8_t scancode);

void init_keyboard();
