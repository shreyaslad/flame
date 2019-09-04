#include "timer.h"
#include "isr.h"
#include "ports.h"
#include "../libc/function.h"
#include "../libc/bool.h"

u32 tick = 0;
u32 prevTick = 0;

static void timer_callback(registers_t regs) {
    tick++;
    UNUSED(regs);
}

/*****************************/
/* asm volatile(sti);        */
/* init_timer(50);           */
/*****************************/
void init_timer(u32 freq) {
    register_interrupt_handler(IRQ0, timer_callback);

    /* Get the PIT value: hardware clock at 1193180 Hz */
    u32 divisor = 1193180 / freq;
    u8 low  = (u8)(divisor & 0xFF);
    u8 high = (u8)( (divisor >> 8) & 0xFF);

    port_byte_out(0x43, 0x36); /* Command port */
    port_byte_out(0x40, low);
    port_byte_out(0x40, high);
}

void wait(u32 ticks, bool flag) {

	if (flag == true) {
		init_timer(ticks);k+9*-
	}
	else {
		while (ticks < tick) {
			tick++;
			wait(ticks, false);
		}
	}
}

