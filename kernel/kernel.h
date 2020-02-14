#pragma once

#include <stddef.h>
#include <string.h>
#include <mm/mm.h>
#include <mm/pmm.h>
#include <math.h>
#include <drivers/screen.h>
#include <drivers/sound.h>
#include <drivers/time.h>
#include <drivers/keyboard.h>
#include <drivers/vga.h>
#include <drivers/serial.h>
#include <fs/hdd.h>
#include <fs/hddw.h>
#include "../cpu/type.h"
#include "../cpu/timer.h"
#include "../cpu/ports.h"
#include "../cpu/isr.h"
#include <boot/multiboot.h>

void user_input(char* input);