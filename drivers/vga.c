#include <drivers/vga.h>

uint8_t horzDisp = 0;
uint8_t vertDisp = 0;

void getResolution() {
    uint8_t bit = port_byte_in(0x3CC);
    set_bit(&bit, 0, 0);
    port_byte_out(0x3C2, bit);

    // TODO: somehow set bit 7 of register 0x11 in port 0x3D4 to allow the use of registers 0x00 - 0x07

    port_byte_out(0x3D4, 0x00);
    horzDisp = port_byte_in(0x3D5);

    set_bit(&bit, 0, 1);
    port_byte_out(0x3C2, bit);
}