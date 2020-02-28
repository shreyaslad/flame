/*
    vfs.h
    Copyright Shreyas Lad (PenetratingShot) 2020

    Virtual File System
*/

#pragma once

#include <stdint.h>
#include <stddef.h>
#include <string.h>

// include the fs later

typedef char* (*read_t)(char*);
typedef void (*write_t)();

typedef struct node_t {
  char* name;
  char* path;
  uint64_t perms;
  uint64_t size;
  uint64_t creationtime;

  read_t rfunc;
  write_t wfunc;
  
  uint64_t type; // 0: Directory | 1. File
  uint64_t nchild;
  struct node_t* parent;
  struct node_t** children;
} node_t;

extern char* currentPath;

void initDirPath();