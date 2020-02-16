#pragma once

#include "../cpu/isr.h"
#include "../cpu/ports.h"
#include "../cpu/timer.h"
#include "../cpu/type.h"
#include <boot/multiboot.h>
#include <drivers/keyboard.h>
#include <drivers/screen.h>
#include <drivers/serial.h>
#include <drivers/sound.h>
#include <drivers/time.h>
#include <drivers/vga.h>
#include <fs/hdd.h>
#include <fs/hddw.h>
#include <math.h>
#include <mm/mm.h>
#include <stddef.h>
#include <string.h>

void user_input(char* input);