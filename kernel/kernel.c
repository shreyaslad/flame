#include "kernel.h"


void kmain() {
	isr_install();
	/*irq_install();

	clear();

	formatTime();

	kprint("Hello There,\n");

	kprint("It is ");
	kprint(format);
	kprint(".");
	kprint("\n>");*/

	asm volatile("sti");
	init_timer(50);
	init_keyboard();
}

void user_input(char* input) {
	if (strcmp(input, "halt") == 0) {
		kprint("Halting CPU");
		asm volatile("hlt");
	}
	else if (strcmp(input, "time") == 0) {
		formatTime();

		kprint(format);
	}
	else if (strcmp(input, "clear") == 0) {
		clear();

		formatTime();

		kprint("Hello There,\n");

		kprint("It is ");
		kprint(format);
		kprint(".");
	}
	else if (strcmp(input, "print") == 0) {
		printf("Hi%s", " there");
	}
	else {
		kprint("Unrecognized command: ");
		kprint(input);
	}
	kprint("\n>");
}