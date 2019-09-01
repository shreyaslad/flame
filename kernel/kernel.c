#include "../cpu/isr.h"
#include "../drivers/screen.h"
#include "kernel.h"
#include "../libc/string.h"
#include "../libc/mem.h"
#include "../libc/bool.h"

void main() {
	isr_install();
	irq_install();

	kprint("Initialized shell\nType HELP to access the help section\n>");
}

void user_input(char* input) {
	if (strcmp(input, "END") == true) {
		kprint("Halting CPU\n");
		asm volatile("hlt");
	}
	else if (strcmp(input, "PAGE") == true) {
		u32 phys_addr;
		u32 page = kmalloc(1000, 1, &phys_addr);
		char page_str[16] = "";
		hex_to_ascii(page, page_str);
		char phys_str[16] = "";
		hex_to_ascii(phys_addr, phys_str);
		kprint("Page: ");
		kprint(page_str);
		kprint(", physical address: ");
		kprint(phys_str);
		kprint("\n");
	}
	kprint("You said: ");
	kprint(input);
	kprint("\n> ");
}