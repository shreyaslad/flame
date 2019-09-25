#include "kernel.h"

void main() {
	isr_install();
	irq_install();

	clear_screen();
	
	kprint("Initialized shell\n>");
}

void user_input(char* input) {
	if (strcmp(input, "end") == true) {

		halt(); // Halt the CPU

	}
	else if (strcmp(input, "test") == true) {
		if (octalToDecimal(13) == 11) {
			kprint("yes");
		}
		else {
			kprint("no");
		}
	}
	else{

		//char string[100]; // reserve 100 bytes for the error message

		//strcpy(string, strappend("Could not find command: ", input));

		//char tgt[len];
		//memory_set(tgt, '\0', sizeof(tgt));
		//strcpy(tgt, string);

		kprint("Couldn't find command"); // Concatenate error message and user input
	}

	kprint("\n> ");
}