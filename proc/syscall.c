/*
  syscall.c
  Copyright Shreyas Lad (PenetratingShot) 2020

  Syscall Handler
*/

#include <proc/syscall.h>

extern void _entersys();

/********
 * MSRS *
 ********/

uint64_t rdmsr(uint64_t msr) {
  uint64_t rax, rdx;
  asm("rdmsr" : "=a"(rax), "=d"(rdx) : "c"(msr));

  return (rdx << 32) | rax;
}

void wrmsr(uint64_t msr, uint64_t data) {
  uint64_t rax = data & 0xFFFFFFFF;
  uint64_t rdx = data >> 32;
  asm("wrmsr" ::"a"(rax), "d"(rdx), "c"(msr));
}
/************
 * Syscalls *
 ************/

syshandler_t syshandlers[] = {sysOpen, sysRead, sysWrite, sysClose};

void initSyscall() {
  wrmsr(EFER, rdmsr(0xC0000080) | 1);
  wrmsr(STAR, CS << 57);
  wrmsr(STAR, SS << 63);
  wrmsr(LSTAR, (uint64_t)_entersys);
}

void syscallHandler(sysregs_t* regs) {
  syshandlers[regs->rax](regs);

  return;
}

void sysOpen(sysregs_t* regs) {

  /*
    Inputs
    ------
    rdi: filepath

    Outputs
    -------
    rax: fd
  */

  node_t* node = vfsResolve((char*)regs->rdi);
  regs->rax = addNode(node);

  return;
}

void sysRead(sysregs_t* regs) {

  /*
    Inputs
    ------
    rdi: fd
    rsi: pointer to buffer
    rdx: bytes to read

    Outputs
    -------
    rax: errno
  */

  node_t* node = vfsResolve((char*)regs->rdi);
  node->rfunc(node, regs->rsi, regs->rdx);

  return;
}

void sysWrite(sysregs_t* regs) {

  /*
    Inputs
    ------
    rdi: fd
    rsi: text to write

    Outputs
    -------
    rax: errno
  */

  node_t* node = vfsResolve((char*)regs->rdi);
  node->wfunc(node, regs->rsi);

  return;
}

void sysClose(sysregs_t* regs) {

  /*
    Inputs
    ------
    rdi: fd

    Outputs
    -------
    rax: errno
  */

  removeNode(regs->rdi);

  return;
}