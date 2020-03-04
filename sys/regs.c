/*
    regs.h
    Copyright Shreyas Lad (PenetratingShot) 2020

    Registers
*/

#include <sys/regs.h>

registers_t* createRegs(uint64_t rip, uint64_t rsp) {
  registers_t* regs = (registers_t*)malloc(sizeof(registers_t));

  regs->r15 = 0;
  regs->r14 = 0;
  regs->r13 = 0;
  regs->r12 = 0;
  regs->r11 = 0;
  regs->r10 = 0;
  regs->r9 = 0;
  regs->r8 = 0;
  regs->rsi = 0;
  regs->rdi = 0;
  regs->rbp = 0;
  regs->rdx = 0;
  regs->rcx = 0;
  regs->rbx = 0;
  regs->rax = 0;
  regs->int_no = -1;
  regs->err_code = -1;
  regs->rip = rip;
  regs->cs = 0;
  regs->rflags = 0;
  regs->rsp = rsp;
  regs->ss = 0;

  return regs;
}