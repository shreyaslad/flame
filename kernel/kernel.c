#include "kernel.h"

multiboot_memory_map_t *mmap;
uint32_t largestUseableMem = 0;

extern uint64_t KNL_CORE_END;

void kmain(uint64_t magic, multiboot_info_t* mbd) {
	disableCursor();

	isr_install();
	irq_install();

	clear();
	drawLogo();
	wait_s(2);
	clear();

	formatTime();

	kprint("flameOS\n");

	kprint("It is ");
	kprint(format);
	kprint(".");
	kprint("\nuser@flameOS # ");

	enableCursor();

	if (mbd->flags & MULTIBOOT_INFO_MEM_MAP) {
		for (mmap = (struct multiboot_mmap_entry *)mbd->mmap_addr; (uint32_t)mmap < (mbd->mmap_addr + mbd->mmap_length); mmap = (struct multiboot_mmap_entry *)((uint32_t)mmap + mmap->size + sizeof(mmap->size))) {
			uint32_t addrH = mmap->addr_high;
			uint32_t addrL = mmap->addr_low;

			uint32_t lenH = mmap->len_high;
			uint32_t lenL = mmap->len_low;

			uint8_t mType = mmap->type;

			if (mType == 1) {
				if (lenL > largestUseableMem) {
					largestUseableMem = abs(lenL - abs(&KNL_CORE_END - addrL));
				}
			}
		}
	}
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
		neofetch(largestUseableMem);
	}
	else if (strcmp(input, "uptime") == 0) {
		calculateUptime();
	}
	else if (strcmp(input, "drives") == 0) {
		drive_scan();
		if (mp == 0 || mp48 == 0) {
			kprint("Primary IDE, Master Drive (Drive 1): Online\n");
		} else {
			kprint("Primary IDE, Master Drive (Drive 1): Offline\n");
		}
		if (ms == 0 || ms48 == 0) {
			kprint("Primary IDE, Slave Drive (Drive 2): Online\n");
		} else {
			kprint("Primary IDE, Slave Drive (Drive 2): Offline\n");
		}
		if (sp == 0 || sp48 == 0) {
			kprint("Secondary IDE, Master Drive (Drive 3): Online\n");
		} else {
			kprint("Secondary IDE, Master Drive (Drive 3): Offline\n");
		}
		if (ss == 0 || ss48 == 0) {
			kprint("Secondary IDE, Slave Drive (Drive 4): Online\n");
		} else {
			kprint("Secondary IDE, Slave Drive (Drive 4): Offline\n");
		}
	}
	else if (strcmp(input, "read") == 0) {
		read_disk(0);
	}
	else if (strcmp(input, "driveTest") == 0) {
		writeIn[0] = 0x1111;
		write(0, 0);
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