/*
  vfs.h
  Copyright Shreyas Lad (PenetratingShot) 2020

  Virtual File System
*/

#pragma once

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/timer.h>
#include <sys/syscall.h>
#include <mm/pmm.h>

// include the fs later

#define DIR 0
#define FILE 1

struct node_t;
typedef struct node_t node_t;
typedef uint64_t type_t;

typedef char* (*read_t)(node_t*, uint64_t);
typedef uint64_t (*write_t)(node_t*, char*);

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
extern node_t* root;

node_t* createNode(char* name, uint64_t perms, type_t type);
void addChild(node_t* parent, node_t* child);
node_t* vfsresolve(char* filepath);
void createNodeInPath(char* path, node_t* newNode);

char* vfsread(node_t* node, uint64_t bytes);
uint64_t vfswrite(node_t* node, char* text);