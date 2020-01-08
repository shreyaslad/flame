#include <drivers/keyboard.h>

#define BACKSPACE 0x0E
#define ENTER 0x1C
#define LSHIFT 0x2a
#define RSHIFT 0x36
#define POWER 0x37

#define RIGHT_ARROW 0x4D
#define LEFT_ARROW 0x4B

#define KEYUPOFFSET 0x80 //128

#define SC_MAX 57

static char key_buffer[256];

int coutkey = 0;
int shift = 0; // is shift key pressed

bool canType = true;

char *sc_name[] = { "ERROR", "Esc", "1", "2", "3", "4", "5", "6", 
    "7", "8", "9", "0", "-", "=", "Backspace", "Tab", "Q", "W", "E", 
        "R", "T", "Y", "U", "I", "O", "P", "[", "]", "Enter", "Lctrl", 
        "A", "S", "D", "F", "G", "H", "J", "K", "L", ";", "'", "`", 
        "LShift", "\\", "Z", "X", "C", "V", "B", "N", "M", ",", ".", 
        "/", "RShift", "Keypad *", "LAlt", "Spacebar"};

// All characters are lowercase for now
char sc_ascii[] = { ' ', ' ', '1', '2', '3', '4', '5', '6',
	'7', '8', '9', '0', '-', '=', ' ', ' ', 'q', 'w', 'e', 'r', 't', 'y',
		'u', 'i', 'o', 'p', '[', ']', ' ', ' ', 'a', 's', 'd', 'f', 'g',
		'h', 'j', 'k', 'l', ';', '\'', '`', ' ', '\\', 'z', 'x', 'c', 'v',
		'b', 'n', 'm', ',', '.', '/', ' ', ' ', ' ', ' ' };

char sc_ascii_uppercase[] = { ' ', ' ', '!', '@', '#', '$', '%', '^',
	'&', '*', '(', ')', '_', '+', ' ', ' ', 'Q', 'W', 'E', 'R', 'T', 'Y',
		'U', 'I', 'O', 'P', '{', '}', ' ', ' ', 'A', 'S', 'D', 'F', 'G',
		'H', 'J', 'K', 'L', ':', '|', '~', ' ', '\\', 'Z', 'X', 'C', 'V',
		'B', 'N', 'M', '<', '>', '?', ' ', ' ', ' ', ' ' };

static void keyboard_callback(registers_t* regs) {

    uint8_t scancode = port_byte_in(0x60);
	if (canType) {
		logic(scancode);
	} else {

	}	

    UNUSED(regs);
}

void logic(uint8_t scancode) {
	bool keyUp = false;

	if (scancode >= KEYUPOFFSET) {
		keyUp = true;
		scancode -= KEYUPOFFSET;
	}

	if (scancode > SC_MAX) return; // Typing a key that somehow doesn't exist

	// Prevent user from deleting text that they didn't place
	if (strcmp(sc_name[scancode], "Backspace") == 0 && keyUp == false) {
		if (coutkey > 0) {
			backspace(key_buffer);
			kprint_backspace();

			coutkey--;
		}
		else {
			return;
		}
	} else if (strcmp(sc_name[scancode], "Enter") == 0 && keyUp == false) {
		kprint("\n");
		user_input(key_buffer);
		key_buffer[0] = '\0';

		coutkey = 0;
	}
	else if (strcmp(sc_name[scancode], "LShift") == 0 && keyUp == false) {
		shift = 1;
	}
	else if (strcmp(sc_name[scancode], "RShift") == 0 && keyUp == false) {
		shift = 1;
	}
	else if (strcmp(sc_name[scancode], "LShift") == 0 && keyUp == true) {
		shift = 0;
	}
	else if (strcmp(sc_name[scancode], "RShift") == 0 && keyUp == true) {
		shift = 0;
	}
	else if (scancode == LEFT_ARROW && keyUp == false) {
		// TODO: insert characters into key_buffer whenever cursor pos is changed

		if (coutkey > 0) {
			int offset = get_cursor_offset();
			set_cursor_offset(get_offset(get_offset_col(offset - 1), get_offset_row(offset)));
		}
		else {
			// don't change the cursor pos
		}
	}
	else if (scancode == RIGHT_ARROW && keyUp == false) {
		
	}
	else if (strcmp(sc_name[scancode], "Spacebar") == 0 && keyUp == false) {
		strcat(key_buffer, " ");
		kprint(" ");
		coutkey++;
	}
	// the arrow keys aren't recognized but they print a scancode larger than 255 :/
	else if (scancode == RIGHT_ARROW) {
		int oldOffset = get_cursor_offset();
		int currentCol = get_offset_col(oldOffset);
		int currentRow = get_offset_row(oldOffset);

		int offset = get_offset(currentCol + 1, currentRow);
		set_cursor_offset(offset);
	}
	else if (scancode == LEFT_ARROW) {
		int oldOffset = get_cursor_offset();
		int currentCol = get_offset_col(oldOffset);
		int currentRow = get_offset_row(oldOffset);

		int offset = get_offset(currentCol - 1, currentRow);
		set_cursor_offset(offset);
	}
	else {
		
		if (shift == 0) {
			if (!keyUp) {
				if (scancode < SC_MAX) {
					coutkey++;
					char letter = sc_ascii[(int)scancode];

					char str[2] = { letter, '\0' };
					append(key_buffer, letter);
					kprint(str);
				}
			}
		}
		else {
			if (!keyUp) {
				coutkey++;
				if (scancode < SC_MAX) {
					char letter = sc_ascii_uppercase[(int)scancode];

					char str[2] = { letter, '\0' };
					append(key_buffer, letter);
					kprint(str);
				}
			}
		}
		
	}
}

void init_keyboard() {
   register_interrupt_handler(IRQ1, keyboard_callback); 
}
