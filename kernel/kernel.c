#include "kernel.h"


void kmain() {
	isr_install();
	irq_install();

	clear();

	read_rtc();
	formatTime();

	kprint("Hello There,\n");

	kprint("It is ");
	kprint(format);
	kprint(".");
	kprint("\n>");
}

void user_input(char* input) {
	if (strcmp(input, "halt") == 0) {
		kprint("Halting CPU");
		asm volatile("hlt");
	}
	else if (strcmp(input, "time") == 0) {
		read_rtc();
		formatTime();

		kprint(format);
	}
	else if (strcmp(input, "clear") == 0) {
		clear();

		read_rtc();
		formatTime();

		kprint("Hello There,\n");

		kprint("It is ");
		kprint(format);
		kprint(".");
	}
	else {
		//TODO: grab only the frist argument (separated by a space)
		char* buf = (char*)malloc(sizeof(char));

		strcpy(buf, "Unrecognized command: ");
		strcat(buf, input);
		kprint(buf);

		free(buf);
	}
	kprint("\n>");
}