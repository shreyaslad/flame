/*
    thread.h
    Copyright Shreyas Lad (PenetratingShot) 2020

    Thread Spawning
*/

#include <proc/thread.h>

uint64_t threadsLen = 0;
uint64_t procsLen = 0;
uint64_t lastFreedThreadIndex =
    NULL; // keep the index of the last thread that was killed
uint64_t lastFreedProcIndex = NULL;

proc_t* currentProcess;
thread_t* currentThread;
thread_t** threads;
proc_t** processes;

threadregs_t* createRegs(uint64_t rip, uint64_t rsp) {
  threadregs_t* regs = (threadregs_t*)malloc(sizeof(threadregs_t));

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
  regs->rip = rip;
  regs->cs = 0;
  regs->rflags = 0;
  regs->rsp = rsp;
  regs->ss = 0;

  return regs;
}

void initScheduler() {
  currentProcess = (proc_t*)malloc(sizeof(proc_t));
  currentThread = (thread_t*)malloc(sizeof(thread_t));
  threads = (thread_t**)malloc(sizeof(thread_t));
  processes = (proc_t**)malloc(sizeof(proc_t));
}

void setCurrentProcess(proc_t* process) { currentProcess = process; }

proc_t* getCurrentProcess() { return currentProcess; }

void setCurrentThread(thread_t* thread) { currentThread = thread; }

thread_t* getCurrentThread() { return currentThread; }

proc_t* spawn(uint64_t rip, uint64_t rsp) {
  procsLen++;

  proc_t* process = (proc_t*)malloc(sizeof(proc_t));

  // ideally we should keep an array of indexes but this should suffice
  if (lastFreedProcIndex = NULL) {
    processes = pmrealloc(processes, procsLen - 1, procsLen);
    processes[procsLen - 1] = process;
    process->pid = procsLen;
  } else {
    processes[lastFreedProcIndex] = process;
    process->pid = lastFreedProcIndex;

    lastFreedProcIndex = NULL;
  }

  fork(process, rip, rsp + (process->nthreads * 0x4000));
  process->pml4 = (uint64_t*)malloc(512);

  return process;
}

// a process will always start with 1 thread
// more threads will be added later on as execution continues
void fork(proc_t* process, uint64_t rip, uint64_t rsp) {
  threadregs_t* regs = createRegs(rip, rsp);

  thread_t* thread = (thread_t*)malloc(sizeof(thread_t));
  thread->regs = regs;
  thread->state = NOT_RUNNING;
  thread->process = process;

  process->nthreads++;
  process->threads = (thread_t**)pmrealloc(
      process->threads, process->nthreads - 1, process->nthreads);
  process->threads[process->nthreads] = thread;

  threadsLen++;

  // ideally we should keep an array of indexes but this should suffice
  if (lastFreedThreadIndex == NULL) {
    threads = (thread_t**)pmrealloc(threads, threadsLen - 1, threadsLen);
    threads[threadsLen] = thread;
    thread->index = threadsLen;
  } else {
    threads[lastFreedThreadIndex] = thread;
    thread->index = lastFreedThreadIndex;

    lastFreedThreadIndex = NULL;
  }
}

void setState(thread_t* thread, uint8_t state) { thread->state = state; }

thread_t* getNextThread() {
  if (currentThread == NULL) {
    return threads[0];
  } else {
    if (currentThread->index == threadsLen) {
      return threads[0];
    }
  }

  return threads[currentThread->index + 1];
}

uint64_t killThread(thread_t* thread) {
  for (uint64_t i = 0; i < threadsLen; i++) {
    if (i == thread->index) {
      threads[threadsLen] = NULL;
      lastFreedThreadIndex = thread->index;

      return SUCCESS;
    }
  }

  return ERR;
}

uint64_t kill(uint64_t pid) {
  proc_t* process = processes[pid];
  processes[pid] = NULL;
  lastFreedProcIndex = pid;

  for (uint64_t i = 0; i < process->nthreads; i++) {
    killThread(process->threads[i]);
  }

  pmfree(process, sizeof(proc_t) + (sizeof(thread_t) * process->nthreads));

  return SUCCESS; // this is pointless idk why it's here
}