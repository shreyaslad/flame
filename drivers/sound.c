/*
	sound.c

	Lets you play a specific frequency from the speaker.
	You can use cpu/timer.h to time intervals between sounds.

	FFMPEG anyone?
*/

#include "sound.h"

#include "../cpu/ports.h"

void play(uint32_t nFrequence) {
	uint32_t Div;
	uint8_t tmp;

	Div = 1193180 / nFrequence;
	port_byte_out(0x43, 0xb6);
	port_byte_out(0x42, (uint8_t)(Div));
	port_byte_out(0x42, (uint8_t)(Div >> 8));

	tmp = port_byte_in(0x61);
	if (tmp != (tmp | 3)) {
		port_byte_out(0x61, tmp | 3);
	}
}

//shtap
void noSound() {
	uint8_t tmp = port_byte_in(0x61) & 0xFC;

	port_byte_out(0x61, tmp);
}

void boot() {
	play(300);
	//wait(10, true);
	noSound();
}