/*
    kernel.c
    Copyright Shreyas Lad (PenetratingShot) 2020

    Kernel
*/

#include <sys/knl/kernel.h>

multiboot_memory_map_t* mmap;

extern uint64_t __kernel_end;
extern void _init_syscall();
extern void userspacePrint();

void kmain(multiboot_info_t* mbd) {

  initMem(mbd);
  /*initFD();
  initVFS();
  initSyscall();
  initScheduler();*/
  
  //isr_install();
  //irq_install();
  
  //vmap((uint64_t)userspacePrint, (uint64_t)userspacePrint - KNL_HIGH_VMA, getPML4(), USER);
  //_initUserspace();
}