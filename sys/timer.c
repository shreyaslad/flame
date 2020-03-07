#include <sys/timer.h>

uint64_t tick = 0;

static void timer_callback(registers_t* regs) {
  tick++;

  thread_t* currentThread = getCurrentThread();

  // TODO: use the thread states somewhere in this algorithm

  if (currentThread == NULL) {
    ; // TODO: jump to /bin/init
  } else {
    memcpy(currentThread->regs, regs, 15);

    thread_t* newThread = getNextThread();

    regs->rip = newThread->regs->rip;
    regs->rsp = newThread->regs->rsp;

    setCurrentThread(newThread);

    proc_t* newProcess = newThread->process;
    setCurrentProcess(newProcess);
    setPML4(newProcess->pml4);

    _chgrip(newThread->regs->rip);
  }
}

void init_timer(uint32_t freq) {
  register_interrupt_handler(IRQ0, timer_callback);

  /* Get the PIT value: hardware clock at 1193180 Hz */
  uint32_t divisor = 1193180 / freq;
  uint8_t low = (uint8_t)(divisor & 0xFF);
  uint8_t high = (uint8_t)((divisor >> 8) & 0xFF);

  outb(0x43, 0x36); /* Command port */
  outb(0x40, low);
  outb(0x40, high);
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