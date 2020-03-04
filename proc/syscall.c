/*
  syscall.c
  Copyright Shreyas Lad (PenetratingShot) 2020

  Syscall Handler
*/

#include <proc/syscall.h>

node_t** fdarr;
uint64_t fdarrentries = 0;

node_t* getNodeFromEntry(uint64_t fd) { return fdarr[fd]; }

void addFD(node_t* node) {
  fdarr[fdarrentries] = node;
  fdarrentries++;
}