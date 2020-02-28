/*
  keyboard.c
  Copyright Shreyas Lad (PenetratingShot) 2020

  Keyboard Driver
*/

#include <devices/keyboard.h>

static char keyBuffer[256];
char lastBuffer[256];

uint8_t coutkey = 0;
uint8_t coutleft = 0;

uint8_t shift = 0; // is shift key pressed

uint8_t state = 0;
uint8_t upArrowPressed = 0;

char* sc_name[] = {"ERROR",     "Esc",     "1", "2", "3", "4",      "5",
                   "6",         "7",       "8", "9", "0", "-",      "=",
                   "Backspace", "Tab",     "Q", "W", "E", "R",      "T",
                   "Y",         "U",       "I", "O", "P", "[",      "]",
                   "Enter",     "Lctrl",   "A", "S", "D", "F",      "G",
                   "H",         "J",       "K", "L", ";", "'",      "`",
                   "LShift",    "\\",      "Z", "X", "C", "V",      "B",
                   "N",         "M",       ",", ".", "/", "RShift", "Keypad *",
                   "LAlt",      "Spacebar"};

// All characters are lowercase for now
char sc_ascii[] = {' ', ' ', '1', '2', '3',  '4', '5', '6',  '7', '8', '9', '0',
                   '-', '=', ' ', ' ', 'q',  'w', 'e', 'r',  't', 'y', 'u', 'i',
                   'o', 'p', '[', ']', ' ',  ' ', 'a', 's',  'd', 'f', 'g', 'h',
                   'j', 'k', 'l', ';', '\'', '`', ' ', '\\', 'z', 'x', 'c', 'v',
                   'b', 'n', 'm', ',', '.',  '/', ' ', ' ',  ' ', ' '};

char sc_ascii_uppercase[] = {
    ' ', ' ', '!', '@', '#', '$', '%', '^', '&', '*', '(', ')', '_', '+',  ' ',
    ' ', 'Q', 'W', 'E', 'R', 'T', 'Y', 'U', 'I', 'O', 'P', '{', '}', ' ',  ' ',
    'A', 'S', 'D', 'F', 'G', 'H', 'J', 'K', 'L', ':', '|', '~', ' ', '\\', 'Z',
    'X', 'C', 'V', 'B', 'N', 'M', '<', '>', '?', ' ', ' ', ' ', ' '};

static void keyboard_callback(registers_t* regs) {
  uint8_t scancode = port_byte_in(0x60);

  if (scancode == LOCK) {
    state = 1;
  } else if (scancode == RIGHT_ARROW && state == 1) {
    state = 0;

    if (coutleft > 0) {
      if (coutleft != 0) {
        coutleft--;

        shiftCursorPos(1, 0);
      }
    }
  } else if (scancode == LEFT_ARROW && state == 1) {
    state = 0;

    if (coutkey > 0) {
      if (coutleft != coutkey) {
        coutleft++;

        shiftCursorPos(-1, 0);
      }
    }
  } else if (scancode == UP_ARROW && state == 1) {
    upArrowPressed = 1;

    shiftCursorPos(coutleft, 0);

    for (int i = 0; i < coutkey; i++) {
      backspace(keyBuffer);
      kprint_backspace();
    } // this should've cleared the key buffer and the line

    kprint(lastBuffer); // put the last string onto the line
    strcpy(keyBuffer, lastBuffer);

    // reset the values so they can be used appropriately
    coutkey = strlen(lastBuffer);
    coutleft = 0;
  } else if (scancode == DOWN_ARROW && state == 1) {
    if (upArrowPressed == 1) {
      upArrowPressed = 0;

      shiftCursorPos(coutleft, 0);

      for (int i = 0; i < coutkey; i++) {
        backspace(keyBuffer);
        kprint_backspace();
      } // clear the line

      // reset the values since there is nothing on the line
      coutkey = 0;
      coutleft = 0;
    }
  } else {
    // just diable the cursor to prevent them from typing
    logic(scancode);
  }

  UNUSED(regs);
}

void logic(uint8_t scancode) {
  bool keyUp = false;

  if (scancode >= KEYUPOFFSET) {
    keyUp = true;
    scancode -= KEYUPOFFSET;
  }

  if (scancode > SC_MAX)
    return;

  if (strcmp(sc_name[scancode], "Backspace") == 0 && keyUp == false) {
    if (coutkey > 0 && coutleft != coutkey) {
      if (coutleft > 0) {
        remove(keyBuffer, coutkey - coutleft);

        shiftCursorPos(coutleft, 0);

        for (int i = 0; i < coutkey; i++) {
          kprint_backspace();
        }

        kprint(keyBuffer);

        shiftCursorPos(-1, 0);
      } else {
        backspace(keyBuffer);
        kprint_backspace();
      }

      coutkey--; // keep this last since the original coutkey is used in
                 // the above conditional
    } else {
      return;
    }
  } else if (strcmp(sc_name[scancode], "Enter") == 0 && keyUp == false) {
    kprint("\n");
    userInput(keyBuffer);
    keyBuffer[0] = '\0';

    coutkey = 0;
  } else if (strcmp(sc_name[scancode], "LShift") == 0 && keyUp == false) {
    shift = 1;
  } else if (strcmp(sc_name[scancode], "RShift") == 0 && keyUp == false) {
    shift = 1;
  } else if (strcmp(sc_name[scancode], "LShift") == 0 && keyUp == true) {
    shift = 0;
  } else if (strcmp(sc_name[scancode], "RShift") == 0 && keyUp == true) {
    shift = 0;
  } else if (strcmp(sc_name[scancode], "Spacebar") == 0 && keyUp == false) {
    strcat(keyBuffer, " ");
    kprint(" ");
    coutkey++;
  } else {
    if (shift == 0) {
      if (!keyUp) {
        if (scancode < SC_MAX) {
          coutkey++;
          char letter = sc_ascii[(int)scancode];

          char str[2] = {letter, '\0'};
          append(keyBuffer, letter);
          kprint(str);
        }
      }
    } else {
      if (!keyUp) {
        coutkey++;
        if (scancode < SC_MAX) {
          char letter = sc_ascii_uppercase[(int)scancode];

          char str[2] = {letter, '\0'};
          append(keyBuffer, letter);
          kprint(str);
        }
      }
    }
  }
}

void init_keyboard() { register_interrupt_handler(IRQ1, keyboard_callback); }
