#ifndef IDT_H
#define IDT_H

#include "type.h"

#define KERNEL_CS 0x08

/* How every interrupt gate (handler) is defined */
typedef struct {
    u16 low_offset; /* Lower 16 bits of handler function address */
    u16 sel; /* Kernel segment selector */
    u8 always0;
    u8 flags; 
    u16 high_offset; 
} __attribute__((packed)) idt_gate_t ;

typedef struct {
    u16 limit;
    u32 base;
} __attribute__((packed)) idt_register_t;

#define IDT_ENTRIES 256
idt_gate_t idt[IDT_ENTRIES];
idt_register_t idt_reg;


void set_idt_gate(int n, u32 handler);
void set_idt();

#endif
