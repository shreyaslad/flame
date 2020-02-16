#include "idt.h"

void set_idt_gate(int n, uint64_t handler, uint8_t flags) {
  idt[n].low_offset = (uint16_t)handler;
  idt[n].sel = KERNEL_CS; /* 8 byte GDT offset into segment 1 */
  idt[n].zero16 = 0;
  idt[n].flags = flags;
  idt[n].middle_offset =
      (uint16_t)middle_16(handler); // these typecasts aren't really needed :/
  idt[n].high_offset = (uint32_t)high_32(handler);
  idt[n].zero32 = 0;
}

void set_idt() {
  idt_reg.base = (uint64_t)&idt;
  idt_reg.limit = IDT_ENTRIES * sizeof(idt_gate_t) - 1;
  /* Don't make the mistake of loading &idt -- always load &idt_reg */
  asm volatile("lidt %0" ::"m"(idt_reg));
}