#ifndef KERNEL_H
#define KERNEL_H

#include "../cpu/isr.h"
#include "../drivers/screen.h"
#include "./commands/command.h"
#include "../cpu/type.h"
#include "../include/string.h"
#include "../include/mem.h"
#include "../include/int.h"

void user_input(char* input);

#endif