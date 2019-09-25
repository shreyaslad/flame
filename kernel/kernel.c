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
		char str[] = "A,B,C";
		kprint(cut(str, ","));
		kprint(cut(NULL, ","));
	}
	else{

		kprint("Couldn't find command");
	}

	kprint("\n> ");
}