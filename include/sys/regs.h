/*
    regs.h
    Copyright Shreyas Lad (PenetratingShot) 2020

    Registers
*/

#pragma once

#include <stdint.h>
#include <stdlib.h>

typedef struct {
  uint64_t r15, r14, r13, r12, r11, r10, r9, r8, rsi, rdi, rbp, rdx, rcx, rbx,
      rax;
  uint64_t int_no, err_code;
  uint64_t rip, cs, rflags, rsp, ss;
} registers_t;

registers_t* createRegs(uint64_t rip, uint64_t rsp);