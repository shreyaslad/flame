#include "kernel.h"

void kmain(multiboot_info_t* mbd) {
	isr_install();
	irq_install();

	canType = false;

	clear();
	drawLogo();
	wait(3);
	clear();

	formatTime();

	kprint("flameOS\n");

	kprint("It is ");
	kprint(format);
	kprint(".");
	kprint("\nuser@flameOS # ");

	canType = true;

	UNUSED(mbd);
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

		kprint("flameOS\n");

		kprint("It is ");
		kprint(format);
		kprint(".");
	}
	else if (strcmp(input, "res") == 0) {
		getResolution();
	}
	else if (strcmp(input, "neofetch") == 0) {
		neofetchLogo();
	}
	else if (strcmp(input, "uptime") == 0) {
		calculateUptime();
	}
	else {
		if (strcmp(input, "") == 0) {

		} else {
			kprint("Unrecognized command: ");
			kprint(input);
		}
	}
	kprint("\nuser@flameOS # ");
}