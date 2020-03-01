/*
  syscall.h
  Copyright Shreyas Lad (PenetratingShot) 2020

  Syscall Handler
*/

#pragma once

#include <stdint.h>
#include <fs/vfs.h>

void syscallHandler(uint64_t rax);