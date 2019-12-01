#include "timer.h"

uint32_t tick = 0;
uint32_t prevTick = 0;

static void timer_callback(registers_t *regs) {
    tick++;
    kprint("Tick: ");
    
    char tick_ascii[256];
    itoa(tick, tick_ascii);
    kprint(tick_ascii);
    kprint("\n");
    UNUSED(regs);
}

/*****************************/
/* asm volatile(sti);        */
/* init_timer(50);           */
/*****************************/
void init_timer(uint32_t freq) {
    register_interrupt_handler(IRQ0, timer_callback);

    /* Get the PIT value: hardware clock at 1193180 Hz */
	uint32_t divisor = 1193180 / freq;
	uint8_t low  = (u8)(divisor & 0xFF);
	uint8_t high = (u8)( (divisor >> 8) & 0xFF);

    port_byte_out(0x43, 0x36); /* Command port */
    port_byte_out(0x40, low);
    port_byte_out(0x40, high);
}
