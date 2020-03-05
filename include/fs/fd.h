/*
  fd.h
  Copyright Shreyas Lad (PenetratingShot) 2020

  File Descriptor
*/

#pragma once

#include <stdint.h>
#include <stdlib.h>
#include <fs/vfs.h>

void initFD();

uint64_t addNode(node_t* node);
void removeNode(uint64_t fd);
node_t* getNode(uint64_t fd);