#include <system.h>
#include "../kernel/kernel.h"

// Call kernel to input external command
void system(char* string) {
	user_input(string);
}
