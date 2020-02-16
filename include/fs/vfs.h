/*
    vfs.h
    Copyright Shreyas Lad (PenetratingShot) 2020

    Virtual File System
*/

#pragma once

#include <stdint.h>
#include <stddef.h>

// include the fs later

typedef char* (*read_t)();
typedef void (*write_t)(char*);
typedef void (*open_t)(char*);
typedef void (*close_t)(char*);

typedef struct file {
  char* name;
  uint64_t perms;
  uint64_t size;

  read_t read;
  write_t write;
  open_t open;
  close_t close;
} file_t;

typedef struct dir {
  char* name;
  uint64_t perms;
  
  struct dir* childDirs;
  struct file* childFiles;
} dir_t;

typedef struct node {
  char* name;

  struct dir* childDirs;
  struct file* childFiles;
} node_t;

extern char* currentDirPath;

void createFile(char* filename);