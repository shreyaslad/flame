/*
  sound.c
  Copyright Shreyas Lad (PenetratingShot) 2020

  Playing specific frequencies through the speakers

*/

#include <drivers/sound.h>

void play(uint32_t nFrequence) {
  uint32_t Div;
  uint8_t tmp;

  Div = 1193180 / nFrequence;
  outb(0x43, 0xb6);
  outb(0x42, (uint8_t)(Div));
  outb(0x42, (uint8_t)(Div >> 8));

  tmp = inb(0x61);
  if (tmp != (tmp | 3)) {
    outb(0x61, tmp | 3);
  }
}

// shtap
void noSound() {
  uint8_t tmp = inb(0x61) & 0xFC;

  outb(0x61, tmp);
}

void boot() {
  play(300);
  // wait(10, true);
  noSound();
}