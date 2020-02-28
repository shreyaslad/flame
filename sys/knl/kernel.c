/*
    kernel.c
    Copyright Shreyas Lad (PenetratingShot) 2020

    Kernel
*/

#include <sys/knl/kernel.h>

multiboot_memory_map_t* mmap;

extern uint64_t __kernel_end;

void kmain(multiboot_info_t* mbd) {
  disableCursor();

  isr_install();
  irq_install();

  initMem(mbd);
  initDirPath();

  sprintf("Bitmap Addr: %d\n", (uint64_t)bitmap);
  sprintf("MBD Total Mem: %d\n", mbd->mem_upper);

  clear();
  drawLogo();
  wait_s(2);

  clear();

  // formatTime(); rewrite this function
  read_rtc();

  kprint("flameOS\n");

  kprint("It is ");
  // kprint(format);
  kprint_int(second);
  kprint(".");
  kprint("\nuser@flameOS # ");

  enableCursor();
}