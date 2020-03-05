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
#include <mm/pmm.h>

// include the fs later

#define DIR 0
#define FILE 1

typedef struct node_t node_t;
typedef uint64_t type_t;

typedef char* (*read_t)(node_t*, char** buffer, uint64_t);
typedef uint64_t (*write_t)(node_t*, char*);

typedef struct node_t {
  char* name;
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

void initVFS();

node_t* createNode(char* name, uint64_t perms, type_t type, node_t* parent);
node_t* vfsResolve(char* filepath);
void createNodeInPath(char* path, node_t* newNode);

char* vfsRead(node_t* node, char** buffer, uint64_t bytes);
uint64_t vfsWrite(node_t* node, char* text);