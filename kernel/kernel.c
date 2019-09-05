#include "../cpu/isr.h"
#include "../drivers/screen.h"
#include "kernel.h"
#include "../libc/string.h"
#include "../libc/mem.h"
#include "../libc/bool.h"
#include "./commands/command.h"

void main() {
	isr_install();
	irq_install();

	
	kprint("Initialized shell\nType HELP to access the help section\n>");
}

void user_input(char* input) {
	if (strcmp(input, "end") == true) {

		halt(); // Halt the CPU

	}

	kprint("\n> ");
}