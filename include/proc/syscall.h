/*
  syscall.h
  Copyright Shreyas Lad (PenetratingShot) 2020

  Syscall Handler
*/

#pragma once

#include <stdint.h>
#include <fs/vfs.h>

/********
 * MSRS *
 ********/

#define EFER 0xC0000080
#define STAR 0xC0000081
#define LSTAR 0xC0000082

#define CS 0x18
#define SS 0x20

uint64_t rdmsr(uint64_t msr);
void wrmsr(uint64_t msr, uint64_t data);

/************
 * Syscalls *
 ************/

typedef struct {
  uint64_t r15, r14, r13, r12, r11, r10, r9, r8, rsi, rdi, rbp, rdx, rcx, rbx,
      rax;
} sysregs_t;

typedef void (*syshandler_t)(sysregs_t*);

void initSyscall();
void syscallHandler(sysregs_t* regs);

void sysOpen(sysregs_t* regs);
void sysRead(sysregs_t* regs);
void sysWrite(sysregs_t* regs);
void sysClose(sysregs_t* regs);