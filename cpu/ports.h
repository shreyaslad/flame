#ifndef PORTS_H
#define PORTS_H

#include "../cpu/type.h"
#include <stdint.h>

uint8_t port_byte_in (uint16_t port);
void port_byte_out (uint16_t port, uint8_t data);
uint16_t port_word_in (uint16_t port);
void port_word_out (uint16_t port, uint16_t data);

void set_bit(uint8_t *input, uint8_t bit, uint8_t state);

#endif
