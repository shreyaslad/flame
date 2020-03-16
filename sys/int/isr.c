#include <sys/int/isr.h>

isr_t interrupt_handlers[256];

/* Can't do this with a loop because we need the address
 * of the function names */
void isr_install() {
  set_idt_gate(0, (uint64_t)isr0, 0x8F);
  set_idt_gate(1, (uint64_t)isr1, 0x8F);
  set_idt_gate(2, (uint64_t)isr2, 0x8F);
  set_idt_gate(3, (uint64_t)isr3, 0x8F);
  set_idt_gate(4, (uint64_t)isr4, 0x8F);
  set_idt_gate(5, (uint64_t)isr5, 0x8F);
  set_idt_gate(6, (uint64_t)isr6, 0x8F);
  set_idt_gate(7, (uint64_t)isr7, 0x8F);
  set_idt_gate(8, (uint64_t)isr8, 0x8F);
  set_idt_gate(9, (uint64_t)isr9, 0x8F);
  set_idt_gate(10, (uint64_t)isr10, 0x8F);
  set_idt_gate(11, (uint64_t)isr11, 0x8F);
  set_idt_gate(12, (uint64_t)isr12, 0x8F);
  set_idt_gate(13, (uint64_t)isr13, 0x8F);
  set_idt_gate(14, (uint64_t)isr14, 0x8F);
  set_idt_gate(15, (uint64_t)isr15, 0x8F);
  set_idt_gate(16, (uint64_t)isr16, 0x8F);
  set_idt_gate(17, (uint64_t)isr17, 0x8F);
  set_idt_gate(18, (uint64_t)isr18, 0x8F);
  set_idt_gate(19, (uint64_t)isr19, 0x8F);
  set_idt_gate(20, (uint64_t)isr20, 0x8F);
  set_idt_gate(21, (uint64_t)isr21, 0x8F);
  set_idt_gate(22, (uint64_t)isr22, 0x8F);
  set_idt_gate(23, (uint64_t)isr23, 0x8F);
  set_idt_gate(24, (uint64_t)isr24, 0x8F);
  set_idt_gate(25, (uint64_t)isr25, 0x8F);
  set_idt_gate(26, (uint64_t)isr26, 0x8F);
  set_idt_gate(27, (uint64_t)isr27, 0x8F);
  set_idt_gate(28, (uint64_t)isr28, 0x8F);
  set_idt_gate(29, (uint64_t)isr29, 0x8F);
  set_idt_gate(30, (uint64_t)isr30, 0x8F);
  set_idt_gate(31, (uint64_t)isr31, 0x8F);

  // Remap the PIC
  outb(0x20, 0x11);
  outb(0xA0, 0x11);
  outb(0x21, 0x20);
  outb(0xA1, 0x28);
  outb(0x21, 0x04);
  outb(0xA1, 0x02);
  outb(0x21, 0x01);
  outb(0xA1, 0x01);
  outb(0x21, 0x0);
  outb(0xA1, 0x0);

  // Install the IRQs
  set_idt_gate(32, (uint64_t)irq0, 0x8E);
  set_idt_gate(33, (uint64_t)irq1, 0x8E);
  set_idt_gate(34, (uint64_t)irq2, 0x8E);
  set_idt_gate(35, (uint64_t)irq3, 0x8E);
  set_idt_gate(36, (uint64_t)irq4, 0x8E);
  set_idt_gate(37, (uint64_t)irq5, 0x8E);
  set_idt_gate(38, (uint64_t)irq6, 0x8E);
  set_idt_gate(39, (uint64_t)irq7, 0x8E);
  set_idt_gate(40, (uint64_t)irq8, 0x8E);
  set_idt_gate(41, (uint64_t)irq9, 0x8E);
  set_idt_gate(42, (uint64_t)irq10, 0x8E);
  set_idt_gate(43, (uint64_t)irq11, 0x8E);
  set_idt_gate(44, (uint64_t)irq12, 0x8E);
  set_idt_gate(45, (uint64_t)irq13, 0x8E);
  set_idt_gate(46, (uint64_t)irq14, 0x8E);
  set_idt_gate(47, (uint64_t)irq15, 0x8E);

  set_idt(); // Load with ASM
}

/* To print the message which defines every exception */
char* exception_messages[] = {"Division By Zero",
                              "Debug",
                              "Non Maskable Interrupt",
                              "Breakpoint",
                              "Into Detected Overflow",
                              "Out of Bounds",
                              "Invalid Opcode",
                              "No Coprocessor",

                              "Double Fault",
                              "Coprocessor Segment Overrun",
                              "Bad TSS",
                              "Segment Not Present",
                              "Stack Fault",
                              "General Protection Fault",
                              "Page Fault",
                              "Unknown Interrupt",

                              "Coprocessor Fault",
                              "Alignment Check",
                              "Machine Check",
                              "Reserved",
                              "Assert Error",
                              "Reserved",
                              "Reserved",
                              "Reserved",

                              "Reserved",
                              "Reserved",
                              "Reserved",
                              "Reserved",
                              "Reserved",
                              "Reserved",
                              "Reserved",
                              "Reserved"};

static uint64_t getCR2() {
  uint64_t cr2;
  asm volatile("movq %%cr2, %0;" : "=r"(cr2));
  return cr2;
}

void isr_handler(registers_t* r) {
  asm volatile("cli");

  //clear();
  printf("Exception %d %d\n", r->int_no, r->err_code);
  sprintf("RIP: %x\nCR2: %x\n", r->rip, getCR2());

  asm volatile("hlt");
}

void register_interrupt_handler(uint8_t n, isr_t handler) {
  interrupt_handlers[n] = handler;
}

void irq_handler(registers_t* r) {
  /* After every interrupt we need to send an EOI to the PICs
   * or they will not send another interrupt again */
  if (r->int_no >= 40)
    outb(0xA0, 0x20); /* slave */
  outb(0x20, 0x20);   /* master */
  if (interrupt_handlers[r->int_no] != 0) {
    isr_t handler = interrupt_handlers[r->int_no];

    handler(r);
  }
}

void irq_install() {
  /* Enable interruptions */
  asm volatile("sti");
  /* IRQ0: timer */
  init_timer(11938);
  /* IRQ1: keyboard */
  init_keyboard();
}
