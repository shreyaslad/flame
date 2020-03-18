/*
Steps to get resolution
1. Set bit 0 of Miscellaneous Output Register (0x3C2) to 0 to allow read access to 0x3D4
2. port_byte_out(0x3D4, 0x01)
3. port_byte_in(0x3D5) gives the Horizontal Display
4. port_byte_out(0x3D4, 0x12)
5. port_byte_in(0x3D5) gives the Vertical Display
*/

#pragma once

#include <stdint.h>
#include <sys/ports.h>
#include <drivers/screen/serial.h>
#include <mm/bit.h>

extern uint8_t horzDisp;

void getResolution();

void enableCursor();
void disableCursor();