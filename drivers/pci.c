/*
    pci.c
    Copyright Shreyas Lad (PenetratingShot) 2020

    PCI Bus Driver
*/

#include <drivers/pci.h>

device_t* createDevice(uint8_t bus, uint8_t slot, uint8_t func) {
  device_t* device = malloc(sizeof(device_t));

  // TODO: create the ID

  device->bus = bus;
  device->slot = slot;
  device->func = func;

  return device;
}

uint32_t createAddress(uint8_t bus, uint8_t slot, uint8_t function,
                       uint16_t offset) {
  return (1 << 31) | (bus << 16) | ((slot & 0b11111) << 11) |
         ((function & 0b111) << 8) | (offset & ~(0b11));
}

uint8_t pciPIOConfigReadB(uint8_t bus, uint8_t slot, uint8_t function,
                          uint16_t offset) {
  outd(PCI_CONFIG_ADDRESS, createAddress(bus, slot, function, offset));

  return inb(PCI_CONFIG_DATA);
}

void pciPIOConfigWriteB(uint8_t bus, uint8_t slot, uint8_t function,
                        uint16_t offset, uint8_t data) {
  outd(PCI_CONFIG_ADDRESS, createAddress(bus, slot, function, offset));
  outb(PCI_CONFIG_DATA, data);
}

uint16_t pciPIOConfigReadW(uint8_t bus, uint8_t slot, uint8_t function,
                           uint16_t offset) {
  outd(PCI_CONFIG_ADDRESS, createAddress(bus, slot, function, offset));

  return inw(PCI_CONFIG_DATA);
}

void pciPIOConfigWriteW(uint8_t bus, uint8_t slot, uint8_t function,
                        uint16_t offset, uint16_t data) {
  outd(PCI_CONFIG_ADDRESS, createAddress(bus, slot, function, offset));
  outw(PCI_CONFIG_DATA, data);
}

uint32_t pciPIOConfigReadD(uint8_t bus, uint8_t slot, uint8_t function,
                           uint16_t offset) {
  outd(PCI_CONFIG_ADDRESS, createAddress(bus, slot, function, offset));

  return ind(PCI_CONFIG_DATA);
}

void pciPIOConfigWriteD(uint8_t bus, uint8_t slot, uint8_t function,
                        uint16_t offset, uint32_t data) {
  outd(PCI_CONFIG_ADDRESS, createAddress(bus, slot, function, offset));
  outd(PCI_CONFIG_DATA, data);
}
