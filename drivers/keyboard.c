#include "keyboard.h"
#include "../cpu/ports.h"
#include "../cpu/isr.h"
#include "screen.h"
#include "../libc/string.h"
#include "../libc/function.h"
#include "../kernel/kernel.h"

#define BACKSPACE 0x0E
#define ENTER 0x1C
#define LSHIFT 0x2a
#define RSHIFT 0x36

static char key_buffer[256];

int coutkey = 0;

#define SC_MAX 57
const char *sc_name[] = { "ERROR", "Esc", "1", "2", "3", "4", "5", "6", 
    "7", "8", "9", "0", "-", "=", "Backspace", "Tab", "Q", "W", "E", 
        "R", "T", "Y", "U", "I", "O", "P", "[", "]", "Enter", "Lctrl", 
        "A", "S", "D", "F", "G", "H", "J", "K", "L", ";", "'", "`", 
        "LShift", "\\", "Z", "X", "C", "V", "B", "N", "M", ",", ".", 
        "/", "RShift", "Keypad *", "LAlt", "Spacebar"};

// All characters are lowercase for now
const char sc_ascii[] = { ' ', ' ', '1', '2', '3', '4', '5', '6',
	'7', '8', '9', '0', '-', '=', ' ', ' ', 'q', 'w', 'e', 'r', 't', 'y',
		'u', 'i', 'o', 'p', '[', ']', ' ', ' ', 'a', 's', 'd', 'f', 'g',
		'h', 'j', 'k', 'l', ';', '\'', '`', ' ', '\\', 'z', 'x', 'c', 'v',
		'b', 'n', 'm', ',', '.', '/', ' ', ' ', ' ', ' ' };

const char sc_ascii_uppercase[] = { ' ', ' ', '!', '@', '#', '$', '%', '^',
	'&', '*', '(', ')', '_', '+', ' ', ' ', 'Q', 'W', 'E', 'R', 'T', 'Y',
		'U', 'I', 'O', 'P', '{', '}', ' ', ' ', 'A', 'S', 'D', 'F', 'G',
		'H', 'J', 'K', 'L', ':', '|', '~', ' ', '\\', 'Z', 'X', 'C', 'V',
		'B', 'N', 'M', '<', '>', '?', ' ', ' ', ' ', ' ' };

static void keyboard_callback(registers_t regs) {

    u8 scancode = port_byte_in(0x60);

	if (scancode > SC_MAX) return; // Typing a key that somehow doesn't exist

	// Prevent user from deleting text that they didn't place
	if (scancode == BACKSPACE) {
		if (coutkey > 0) {
			backspace(key_buffer);
			kprint_backspace();

			coutkey--;
		}
		else {
			return;
		}
		
	} else if (scancode == ENTER) {
		kprint("\n");
		user_input(key_buffer);
		key_buffer[0] = '\0';

		coutkey = 0;
	}
	else {
		coutkey++;
		
		char letter = sc_ascii[(int)scancode];

		char str[2] = { letter, '\0' };
		append(key_buffer, letter);
		kprint(str);
	}

    UNUSED(regs);
}

void init_keyboard() {
   register_interrupt_handler(IRQ1, keyboard_callback); 
}