#pragma once

#include <stdint.h>
#include <stdarg.h>
#include <string.h>
#include <mm/mm.h>
#include <drivers/colors.h>
#include <drivers/time.h>
#include <drivers/serial.h>
#include <drivers/keyboard.h>
#include <fs/hdd.h>
#include <fs/hddw.h>
#include "../../cpu/type.h"
#include "../../cpu/ports.h"

// VGA Output Buffer
#define VIDEO_ADDRESS 0xb8000

// Screen Dimensions
#define MAX_ROWS 25 //25
#define MAX_COLS 80 //80

// Common Color Codes
#define WHITE_ON_BLACK 0x0f
#define RED_ON_WHITE 0xf4

/* Screen i/o ports */
#define REG_SCREEN_CTRL 0x3d4
#define REG_SCREEN_DATA 0x3d5

/* Cursor */
int get_cursor_offset();
void set_cursor_offset(int offset);
void shiftCursorPos(int coffset, int roffset);

/* Expose private cursor pos API */
int get_offset(int col, int row);
int get_offset_row(int offset);
int get_offset_col(int offset);

/* Public kernel API */
void clear();
void kprint_at(char *message, int col, int row);
void kprint(char *message);
void printf(char* message, ...);
void kprint_backspace();
void kprint_int(int num);
void kprint_uint8(uint8_t num);

void kprint_at_color(char *message, int col, int row, char color);
void kprint_color(char* message, char color);

void drawRect(uint8_t color);
void drawLogo();
void neofetch(uint32_t mem);