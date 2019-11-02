#pragma once

#include "../cpu/ports.h"
#include <stdint.h>
#include <string.h>

#define CURRENT_YEAR 2019

extern int century_register;

extern uint8_t second;
extern uint8_t minute;
extern uint8_t hour;
extern uint8_t day;
extern uint8_t month;
extern uint32_t year;

extern char* format;
extern uint16_t formatLen;

enum cmos {
	cmos_address = 0x70,
	cmos_data = 0x71,
};

int get_update_in_progress_flag();
uint8_t get_RTC_register(int reg);
void read_rtc();

void formatTime();