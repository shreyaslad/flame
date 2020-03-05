/*
    thread.h
    Copyright Shreyas Lad (PenetratingShot) 2020

    Thread Spawning
*/

#pragma once

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <mm/pmm.h>
#include <mm/vmm.h>

/*
* Thread States
* -------------
* 0: Thread is not in the thread listing, add it
* 1: Thread is busy spinning
* 2: Thread is next in line to be executed
* 3: Thread is currently running
* 4: Thread has finished executing, remove it 
*/

#define NOT_RUNNING 0
#define IDLE 1
#define READY 2
#define RUNNING 3
#define FINISHED 4

#define SUCCESS 0
#define ERR -1

/* Registers */

typedef struct {
  uint64_t r15, r14, r13, r12, r11, r10, r9, r8, rsi, rdi, rbp, rdx, rcx, rbx,
      rax;
  uint64_t rip, cs, rflags, rsp, ss;
} threadregs_t;

threadregs_t* createRegs(uint64_t rip, uint64_t rsp);

/* Processes and Threads */

typedef struct proc_t proc_t;

typedef struct {
    threadregs_t* regs;
    uint8_t state;
    uint64_t index; // index into threads list

    proc_t* process;
} thread_t;

typedef struct proc_t{
    uint64_t pid; // index into process array
    uint64_t* pml4;

    uint64_t nthreads;
    thread_t** threads;
} proc_t;

void initScheduler();

void setCurrentProcess(proc_t* process);
proc_t* getCurrentProcess();
void setCurrentThread(thread_t* thread);
thread_t* getCurrentThread();

proc_t* spawn(uint64_t rip, uint64_t rsp); // spawns a process
void fork(proc_t* process, uint64_t rip, uint64_t rsp); // creates another thread

void setState(thread_t* thread, uint8_t state);

thread_t* getNextThread();

uint64_t killThread(thread_t* thread);
uint64_t kill(uint64_t pid);