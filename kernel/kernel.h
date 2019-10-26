#ifndef KERNEL_H
#define KERNEL_H

#include "../cpu/isr.h"
#include "../drivers/screen.h"
#include "./commands/command.h"
#include "../cpu/type.h"
#include <string.h>
#include <mem.h>
#include <int.h>
#include <null.h>

void user_input(char* input);

#endif