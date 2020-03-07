#include <devices/screen/screen.h>

int print_char(char c, int col, int row, char attr);
int get_offset(int col, int row);
int get_offset_row(int offset);
int get_offset_col(int offset);

/**********************************************************
 * Public Kernel API functions                            *
 **********************************************************/

void kprint_at(char* message, int col, int row) {
  /* Set cursor if col/row are negative */
  int offset;
  if (col >= 0 && row >= 0)
    offset = get_offset(col, row);
  else {
    offset = get_cursor_offset();
    row = get_offset_row(offset);
    col = get_offset_col(offset);
  }
  int i = 0;
  while (message[i] != 0) {
    offset = print_char(message[i++], col, row, WHITE_ON_BLACK);
    row = get_offset_row(offset);
    col = get_offset_col(offset);
  }
}

void kprint(char* message) { kprint_at(message, -1, -1); }

void kprint_at_color(char* message, int col, int row, char color) {
  /* Set cursor if col/row are negative */
  int offset;
  if (col >= 0 && row >= 0)
    offset = get_offset(col, row);
  else {
    offset = get_cursor_offset();
    row = get_offset_row(offset);
    col = get_offset_col(offset);
  }
  int i = 0;
  while (message[i] != 0) {
    offset = print_char(message[i++], col, row, color);
    row = get_offset_row(offset);
    col = get_offset_col(offset);
  }
}

void kprint_color(char* message, char color) {
  kprint_at_color(message, -1, -1, color);
}

void kprint_backspace() {
  int offset = get_cursor_offset() - 2;
  int row = get_offset_row(offset);
  int col = get_offset_col(offset);
  print_char(0x08, col, row, WHITE_ON_BLACK);
}

void kprint_int(uint64_t num) {
  char buf[65];
  itoa(num, buf);
  kprint(buf);
}

void kprint_uint8(uint8_t num) {
  char buf[33];
  itoa_uint8(num, buf);
  kprint(buf);
}

/**********************************************************
 * Private kernel functions                               *
 **********************************************************/

int print_char(char c, int col, int row, char attr) {
  uint8_t* vidmem = (uint8_t*)VIDEO_ADDRESS;
  if (!attr)
    attr = WHITE_ON_BLACK;

  if (col >= MAX_COLS || row >= MAX_ROWS) {
    vidmem[2 * (MAX_COLS) * (MAX_ROWS)-2] = 'E';
    vidmem[2 * (MAX_COLS) * (MAX_ROWS)-1] = RED_ON_WHITE;
    return get_offset(col, row);
  }

  int offset;
  if (col >= 0 && row >= 0)
    offset = get_offset(col, row);
  else
    offset = get_cursor_offset();

  if (c == '\n') {
    row = get_offset_row(offset);
    offset = get_offset(0, row + 1);
  } else if (c == 0x08) {
    vidmem[offset] = ' ';
    vidmem[offset + 1] = attr;
  } else {
    vidmem[offset] = c;
    vidmem[offset + 1] = attr;
    offset += 2;
  }

  if (offset >= MAX_ROWS * MAX_COLS * 2) {
    for (int i = 1; i < MAX_ROWS; i++)
      memcpy((uint8_t*)(get_offset(0, i) + VIDEO_ADDRESS),
             (uint8_t*)(get_offset(0, i - 1) + VIDEO_ADDRESS), MAX_COLS * 2);

    char* last_line =
        (char*)(get_offset(0, MAX_ROWS - 1) + (uint8_t*)VIDEO_ADDRESS);
    for (int i = 0; i < MAX_COLS * 2; i++)
      last_line[i] = 0;

    offset -= 2 * MAX_COLS;
  }

  set_cursor_offset(offset);
  return offset;
}

int get_cursor_offset() {
  /* Use the VGA ports to get the current cursor position
   * 1. Ask for high byte of the cursor offset (data 14)
   * 2. Ask for low byte (data 15)
   */
  outb(REG_SCREEN_CTRL, 14);
  int offset = inb(REG_SCREEN_DATA) << 8; /* High byte: << 8 */
  outb(REG_SCREEN_CTRL, 15);
  offset += inb(REG_SCREEN_DATA);
  return offset * 2; /* Position * size of character cell */
}

void set_cursor_offset(int offset) {
  offset /= 2;
  outb(REG_SCREEN_CTRL, 14);
  outb(REG_SCREEN_DATA, (uint8_t)(offset >> 8));
  outb(REG_SCREEN_CTRL, 15);
  outb(REG_SCREEN_DATA, (uint8_t)(offset & 0xff));
}

void clear() {
  int screen_size = MAX_COLS * MAX_ROWS;
  int i;
  uint8_t* screen = (uint8_t*)VIDEO_ADDRESS;

  for (i = 0; i < screen_size; i++) {
    screen[i * 2] = ' ';
    screen[i * 2 + 1] = WHITE_ON_BLACK;
  }
  set_cursor_offset(get_offset(0, 0));
}

void shiftCursorPos(int coffset, int roffset) {
  int oldOffset = get_cursor_offset();
  int currentCol = get_offset_col(oldOffset);
  int currentRow = get_offset_row(oldOffset);

  int offset = get_offset(currentCol + coffset, currentRow + roffset);
  set_cursor_offset(offset);
}

int get_offset(int col, int row) { return 2 * (row * MAX_COLS + col); }
int get_offset_row(int offset) { return offset / (2 * MAX_COLS); }
int get_offset_col(int offset) {
  return (offset - (get_offset_row(offset) * 2 * MAX_COLS)) / 2;
}

int logo[16][16] = {{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
                    {0, 0, 0, 3, 0, 0, 0, 0, 0, 0, 0, 3, 3, 0, 0, 0},
                    {0, 0, 3, 3, 0, 0, 0, 0, 0, 0, 3, 2, 3, 0, 0, 0},
                    {0, 0, 3, 3, 0, 0, 0, 0, 0, 3, 3, 1, 2, 3, 0, 0},
                    {0, 3, 1, 2, 3, 0, 0, 0, 3, 2, 2, 2, 2, 3, 0, 0},
                    {0, 3, 2, 2, 1, 3, 0, 3, 2, 1, 2, 1, 2, 2, 3, 0},
                    {0, 3, 2, 2, 2, 3, 0, 3, 2, 2, 2, 2, 1, 2, 3, 0},
                    {3, 2, 1, 1, 2, 2, 3, 2, 1, 1, 1, 2, 2, 2, 3, 0},
                    {3, 2, 2, 1, 1, 2, 2, 1, 2, 1, 1, 2, 2, 2, 3, 0},
                    {3, 2, 2, 1, 2, 1, 1, 1, 1, 1, 1, 2, 2, 2, 3, 0},
                    {0, 3, 2, 1, 1, 2, 1, 1, 1, 1, 2, 2, 2, 3, 3, 0},
                    {0, 0, 3, 1, 1, 1, 1, 1, 2, 1, 1, 1, 3, 3, 0, 0},
                    {0, 0, 0, 3, 3, 1, 1, 1, 1, 1, 1, 1, 3, 0, 0, 0},
                    {0, 0, 0, 0, 3, 3, 1, 1, 1, 1, 1, 3, 0, 0, 0, 0},
                    {0, 0, 0, 0, 0, 3, 1, 1, 1, 1, 3, 0, 0, 0, 0, 0},
                    {0, 0, 0, 0, 0, 0, 3, 3, 3, 3, 0, 0, 0, 0, 0, 0}};

void drawLogo() {
  int xOff = 30;
  int yOff = 2;
  for (int y = 0; y < 16; y++) {
    for (int x = 0; x < 16; x++) {
      if (logo[y][x] == 0) {
        kprint_at_color("  ", x + xOff, y + yOff, BLACK_ON_BLACK);
      } else if (logo[y][x] == 1) {
        kprint_at_color(
            "  ", x + xOff, y + yOff,
            vga_entry_color(VGA_COLOR_LIGHT_RED, VGA_COLOR_LIGHT_RED));
      } else if (logo[y][x] == 2) {
        kprint_at_color(
            "  ", x + xOff, y + yOff,
            vga_entry_color(VGA_COLOR_LIGHT_BROWN, VGA_COLOR_LIGHT_BROWN));
      } else {
        kprint_at_color("  ", x + xOff, y + yOff,
                        vga_entry_color(VGA_COLOR_WHITE, VGA_COLOR_WHITE));
      }
    }
  }
  kprint("\n\n                                    flame");
}

void drawRect(uint8_t color) {
  for (int i = 0; i < 3; i++) {
    kprint_color(" ", color);
  }
}

void neofetch(uint64_t mem) {
  clear();
  formatTime();

  kprint("flameOS\n");

  kprint("It is ");
  kprint(format);
  kprint(".");
  kprint("\nuser@flameOS # neofetch");

  for (int y = 0; y < 16; y++) {
    for (int x = 0; x < 16; x++) {
      if (x == 15)
        kprint("\n");
      if (logo[y][x] == 0) {
        kprint_color("  ", BLACK_ON_BLACK);
      } else if (logo[y][x] == 1) {
        kprint_color("  ",
                     vga_entry_color(VGA_COLOR_LIGHT_RED, VGA_COLOR_LIGHT_RED));
      } else if (logo[y][x] == 2) {
        kprint_color("  ", vga_entry_color(VGA_COLOR_LIGHT_BROWN,
                                           VGA_COLOR_LIGHT_BROWN));
      } else {
        kprint_color("  ", vga_entry_color(VGA_COLOR_WHITE, VGA_COLOR_WHITE));
      }
    }
  }

  int oldOffset = get_cursor_offset();
  int currentCol = get_offset_col(oldOffset);
  int currentRow = get_offset_row(oldOffset);

  int offset = get_offset(currentCol + 35, currentRow - 14);
  set_cursor_offset(offset);
  kprint_color("user@flameOS",
               vga_entry_color(VGA_COLOR_LIGHT_RED, VGA_COLOR_BLACK));

  offset = get_offset(currentCol + 35, currentRow - 13);
  set_cursor_offset(offset);
  kprint("------------");

  offset = get_offset(currentCol + 35, currentRow - 12);
  set_cursor_offset(offset);
  kprint_color("OS: ", vga_entry_color(VGA_COLOR_LIGHT_RED, VGA_COLOR_BLACK));
  kprint("flameOS (x86_64)");

  offset = get_offset(currentCol + 35, currentRow - 11);
  set_cursor_offset(offset);
  kprint_color("Uptime: ",
               vga_entry_color(VGA_COLOR_LIGHT_RED, VGA_COLOR_BLACK));
  calculateUptime();

  offset = get_offset(currentCol + 35, currentRow - 10);
  set_cursor_offset(offset);
  kprint_color("Available Memory: ",
               vga_entry_color(VGA_COLOR_LIGHT_RED, VGA_COLOR_BLACK));
  kprint_int(mem / 1000000);
  kprint("Mb");

  /*offset = get_offset(currentCol + 35, currentRow - 9);
  set_cursor_offset(offset);
  kprint_color("Drive Status: ",
               vga_entry_color(VGA_COLOR_LIGHT_RED, VGA_COLOR_BLACK));
  if (hdd.mp == 0 || hdd.mp48 == 0) {
    kprint_color(" 1 ", vga_entry_color(VGA_COLOR_WHITE, VGA_COLOR_GREEN));
  } else {
    kprint_color(" 1 ", vga_entry_color(VGA_COLOR_WHITE, VGA_COLOR_LIGHT_RED));
  }

  if (hdd.ms == 0 || hdd.ms48 == 0) {
    kprint_color(" 2 ", vga_entry_color(VGA_COLOR_WHITE, VGA_COLOR_GREEN));
  } else {
    kprint_color(" 2 ", vga_entry_color(VGA_COLOR_WHITE, VGA_COLOR_LIGHT_RED));
  }

  if (hdd.sp == 0 || hdd.sp48 == 0) {
    kprint_color(" 3 ", vga_entry_color(VGA_COLOR_WHITE, VGA_COLOR_GREEN));
  } else {
    kprint_color(" 3 ", vga_entry_color(VGA_COLOR_WHITE, VGA_COLOR_LIGHT_RED));
  }

  if (hdd.ss == 0 || hdd.ss48 == 0) {
    kprint_color(" 4 ", vga_entry_color(VGA_COLOR_WHITE, VGA_COLOR_GREEN));
  } else {
    kprint_color(" 4 ", vga_entry_color(VGA_COLOR_WHITE, VGA_COLOR_LIGHT_RED));
  }*/

  offset = get_offset(currentCol + 35, currentRow - 8);
  set_cursor_offset(offset);
  kprint_color("Clock Speed: ",
               vga_entry_color(VGA_COLOR_LIGHT_RED, VGA_COLOR_BLACK));
  kprint_int(DIVISOR / 1193);
  kprint(" Mhz");

  offset = get_offset(currentCol + 35, currentRow - 6);
  set_cursor_offset(offset);
  drawRect(vga_entry_color(VGA_COLOR_DARK_GREY, VGA_COLOR_DARK_GREY));
  drawRect(vga_entry_color(VGA_COLOR_LIGHT_RED, VGA_COLOR_LIGHT_RED));
  drawRect(vga_entry_color(VGA_COLOR_LIGHT_GREEN, VGA_COLOR_LIGHT_GREEN));
  drawRect(vga_entry_color(VGA_COLOR_LIGHT_BROWN, VGA_COLOR_LIGHT_BROWN));
  drawRect(vga_entry_color(VGA_COLOR_BLUE, VGA_COLOR_BLUE));
  drawRect(vga_entry_color(VGA_COLOR_LIGHT_BLUE, VGA_COLOR_LIGHT_BLUE));
  drawRect(vga_entry_color(VGA_COLOR_CYAN, VGA_COLOR_CYAN));
  drawRect(vga_entry_color(VGA_COLOR_WHITE, VGA_COLOR_WHITE));

  set_cursor_offset(oldOffset);
}