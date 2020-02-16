#include "timer.h"

uint32_t tick = 0;

static void timer_callback(registers_t* regs) {
  tick++;
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
  uint8_t low = (u8)(divisor & 0xFF);
  uint8_t high = (u8)((divisor >> 8) & 0xFF);

  port_byte_out(0x43, 0x36); /* Command port */
  port_byte_out(0x40, low);
  port_byte_out(0x40, high);
}

void wait(uint32_t ticks) {
  uint32_t prev = tick;

  while (tick < prev + ticks) {
    ;
    if (tick == prev + ticks)
      break;
  }
}

// this is so dumb but im going to do it anyway
// can't wait more than 60 seconds but fite me
void wait_s(uint32_t seconds) {
  read_rtc();

  uint16_t targetSeconds = 0;
  uint16_t initialSeconds = second;

  if (initialSeconds + seconds > 60) {
    targetSeconds = (initialSeconds + seconds) - 60;
  } else {
    targetSeconds = initialSeconds + seconds;
  }

  while (second != targetSeconds) {
    read_rtc();

    if (second == targetSeconds)
      break;
  }
}