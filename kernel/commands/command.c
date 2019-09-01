#include "command.h"
#include "../../drivers/screen.h"

void halt() {
	kprint("Halting CPU\n");
	asm volatile("hlt");
}