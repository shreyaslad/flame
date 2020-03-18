#include <drivers/screen/serial.h>



void init_serial() {
  outb(PORT + 1, 0x00); // Disable all interrupts
  outb(PORT + 3, 0x80); // Enable DLAB (set baud rate divisor)
  outb(PORT + 0, 0x03); // Set divisor to 3 (lo byte) 38400 baud
  outb(PORT + 1, 0x00); //                  (hi byte)
  outb(PORT + 3, 0x03); // 8 bits, no parity, one stop bit
  outb(PORT + 2,
                0xC7); // Enable FIFO, clear them, with 14-byte threshold
  outb(PORT + 4, 0x0B); // IRQs enabled, RTS/DSR set
}

int is_transmit_empty() { return inb(PORT + 5) & 0x20; }

void write_serial(char a) {
  while (is_transmit_empty() == 0)
    ;

  outb(PORT, a);
}

void sprint(char* message) {
  int i = 0;
  while (message[i] != 0) {
    write_serial(message[i++]);
  }
}

void sprint_int(int num) {
  char toprint[33];
  itoa(num, toprint);
  sprint(toprint);
}

void sprint_uint(unsigned int num) {
  char toprint[33];
  itoa(num, toprint);
  sprint(toprint);
}

void sprintd(char* message) {
  sprint("[");
  sprint_uint(tick);
  sprint(" ticks] [flame:debug]: ");
  sprint(message);
  sprint("\n");
}