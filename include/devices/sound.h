#pragma once

#include <stdint.h>
#include <stdbool.h>
#include <sys/timer.h>
#include <sys/ports.h>

void play(uint32_t nFrequency);
void noSound();

void boot();