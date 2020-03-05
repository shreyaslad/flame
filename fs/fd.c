/*
  fd.c
  Copyright Shreyas Lad (PenetratingShot) 2020

  File Descriptor
*/

#include <fs/fd.h>

node_t** fdarr;
uint64_t fdarrentries = 0;
uint64_t lastFreedEntry = NULL;

void initFD() { fdarr = (node_t**)malloc(sizeof(node_t) * 0); }

uint64_t addNode(node_t* node) {
  if (lastFreedEntry != NULL) {
    fdarrentries++;
    pmrealloc(fdarr, fdarrentries - 1, fdarrentries);

    fdarr[fdarrentries] = node;

    return fdarrentries;
  } else {
    fdarr[lastFreedEntry] = node;
    lastFreedEntry = NULL;
  }
}

void removeNode(uint64_t fd) { fdarr[fdarrentries] = NULL; }

node_t* getNode(uint64_t fd) { return fdarr[fdarrentries]; }