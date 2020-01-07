#pragma once

#include <drivers/screen.h>
#include <drivers/sound.h>
#include <drivers/time.h>
#include <drivers/keyboard.h>
#include "../cpu/type.h"
#include "../cpu/timer.h"
#include "../cpu/ports.h"
#include "../cpu/isr.h"
#include "../multiboot.h"
#include <string.h>
#include <mem.h>
#include <int.h>
#include <stddef.h>

void user_input(char* input);