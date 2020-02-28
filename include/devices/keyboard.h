/*
  keyboard.c
  Copyright Shreyas Lad (PenetratingShot) 2020

  Keyboard Driver
*/

#pragma once

#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include <sys/knl/terminal.h>
#include <sys/ports.h>
#include <sys/int/idt.h>
#include <devices/screen/screen.h>

#define BACKSPACE 0x0E
#define ENTER 0x1C
#define LSHIFT 0x2a
#define RSHIFT 0x36

#define RIGHT_ARROW 0x4D
#define LEFT_ARROW 0x4B
#define UP_ARROW 0x48
#define DOWN_ARROW 0x50

#define KEYUPOFFSET 0x80 //128
#define SC_MAX 57
#define LOCK 0xE0

extern uint8_t coutkey;
extern uint8_t coutleft;

void logic(uint8_t scancode);
void init_keyboard();
