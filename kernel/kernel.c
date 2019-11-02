#include "kernel.h"


void main() {
	isr_install();
	irq_install();

	clear();
	
	kprint("OS\n>");
}

void user_input(char* input) {
	if (strcmp(input, "end") == 0) {
		kprint("Halting CPU");
		asm volatile("hlt");

		// I have no idea what I'm doing
		/*__asm__("mov %eax, 0x1000;"
			"movl %eax, %ss;"
			"movl %esp, 0xf000;"
			"movl %eax, 0x5307;"
			"movl %ebx, 0x0001;"
			"movl %ecx, 0x0003;"
			"int $0x15;"
		);*/
	}
	else {
		char* buf = (char*)malloc(sizeof(char));

		strcpy(buf, "Unrecognized command: ");
		strcat(buf, input);
		kprint(buf);

		free(buf);
	}
	kprint("\n>");
}