/*
	sound.c

	Lets you play a specific frequency from the speaker.
	You can use cpu/timer.h to time intervals between sounds.

	FFMPEG anyone?
*/

#include "sound.h"

#include "../cpu/ports.h"

static void playSound(u32 nFrequency) {
	u32 Div;
	u8 tmp;

	// Set PIT to frequency
	Div = 1193180 / nFrequency;
	port_byte_out(0x43, 0xb6);
	port_byte_out(0x42, (u8)(Div));
	port_byte_out(0x42, (u8)(Div >> 8));

	tmp = port_byte_in(0x61);
	if (tmp != (tmp | 3)) {
		port_byte_out(0x61, tmp | 3);
	}
}

//shtap
static void noSound() {
	u8 tmp = port_byte_in(0x61) & 0xFC;

	port_byte_out(0x61, tmp);
}