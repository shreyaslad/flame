#include <devices/screen/vga.h>

uint8_t horzDisp = 0;

void getResolution() {
  // set bit 0 of 0x3C2 to 0 to allow access to 0x3D4 and 0x3D5
  uint8_t bit = inb(0x3CC);
  setBit(&bit, 0, 0);
  outb(0x3C2, bit);

  // index 0x07 stores overflow (high order bits) for vertical timings (0x06 -
  // 0x15) set bit 7 of index 0x11 of port 0x3D4 to 0

  // request 0x3D4 to send the contents of register 0x11 to 0x3D5
  // read the content from 0x3D5
  // set bit 7 to 0
  // write the modified value to 0x3D5
  outb(0x3D4, 0x11);
  uint8_t pbit = inb(0x3D5);
  setBit(&pbit, 7, 0);
  outb(0x3D5, pbit);

  // now you have access to 0x3D4
  // request the horizontal total from 0x3D4
  outb(0x3D4, 0x00);
  horzDisp = inb(0x3D5);

  // reset the protect bit
  outb(0x3D4, 0x11);
  setBit(&pbit, 7, 1);
  outb(0x3D5, pbit);

  // reset bit 0 in 0x3C2
  setBit(&bit, 0, 1);
  outb(0x3C2, bit);
}

void enableCursor() {
  /* Set cursor scanline begin to 0 and make cursor visible */
  outb(0x3D4, 0x0A);
  outb(0x3D5, inb(0x3D5) & 0xC0);

  /* Set cursor scanline end to 15 */
  outb(0x3D4, 0x0B);
  outb(0x3D5, (inb(0x3D5) & 0xE0) | 15);
}

void disableCursor() {
  outb(0x3D4, 0x0A);
  outb(0x3D5, 0x20);
}