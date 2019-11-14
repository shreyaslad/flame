#ifndef SCREEN_H
#define SCREEN_H

#include "../cpu/type.h"
#include "../cpu/ports.h"
#include "../include/mem.h"

// VGA Output Buffer
#define VIDEO_ADDRESS 0xb8000

// Screen Dimensions
#define MAX_ROWS 25
#define MAX_COLS 80

// Common Color Codes
#define WHITE_ON_BLACK 0x0f
#define RED_ON_WHITE 0xf4

/* Screen i/o ports */
#define REG_SCREEN_CTRL 0x3d4
#define REG_SCREEN_DATA 0x3d5

/* Cursor */
int get_cursor_offset();
void set_cursor_offset(int offset);

/* Expose private cursor pos API */
int get_offset(int col, int row);
int get_offset_row(int offset);
int get_offset_col(int offset);

/* Public kernel API */
void clear();
void kprint_at(char *message, int col, int row);
void kprint(char *message);
void kprint_backspace();

#endif
