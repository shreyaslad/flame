/*
    kernel.c
    Copyright Shreyas Lad (PenetratingShot) 2020

    Kernel
*/

#include <sys/knl/kernel.h>

multiboot_memory_map_t* mmap;

extern uint64_t _kernel_end;
extern void _init_syscall();
extern void userspacePrint();

void kmain(multiboot_info_t* mbd) {
  disableCursor();
  initMem(mbd);
  /*initFD();
  initVFS();
  initSyscall();
  initScheduler();*/
  
  isr_install();
  sprintd("Initialized ISRS");

  irq_install();
  sprintd("Initialized IRQS");

  drawLogo();
  wait_s(2);

  clear();
  enableCursor();

  formatTime();
  printf("flame\nIt is %s\nuser@flame ~$ ", format);

  //vmap((uint64_t)userspacePrint, (uint64_t)userspacePrint - KNL_HIGH_VMA, getPML4(), USER);
  //_initUserspace();
}