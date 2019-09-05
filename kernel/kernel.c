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

	
	kprint("Initialized shell\n>");
}

void user_input(char* input) {
	if (strcmp(input, "end") == true) {

		halt(); // Halt the CPU

	}
	else {

		char string[100]; // reserve 100 bytes for the error message

		strcpy(string, strappend("Could not find command: ", input));

		//char tgt[len];
		//memory_set(tgt, '\0', sizeof(tgt));
		//strcpy(tgt, string);

		kprint(string); // Concatenate error message and user input
	}

	kprint("\n> ");
}