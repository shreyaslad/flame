/*
    pci.c
    Copyright Shreyas Lad (PenetratingShot) 2020

    PCI Bus Driver
*/

#include <devices/pci.h>

device_t* createDevice(uint32_t bus, uint32_t slot, uint32_t func) {
  device_t* device = malloc(sizeof(device_t));

  device->bus = bus;
  device->slot = slot;
  device->func = func;

  return device;
}

uint32_t createAddress(device_t* device, uint32_t offset) {
  return (1 << 31) | (device->bus << 16) | ((device->slot & 0b11111) << 11) |
         ((device->func & 0b111) << 8) | (offset & ~(0b11));
}

uint32_t pciPIOConfigRead(device_t* device, uint32_t offset) {
  outd(PCI_CONFIG_ADDRESS, createAddress(device, offset));

  return ind(PCI_CONFIG_DATA);
}

void pciPIOConfigWrite(device_t* device, uint32_t offset, uint32_t data) {
  outd(PCI_CONFIG_ADDRESS, createAddress(device, offset));
  outd(PCI_CONFIG_DATA, data);
}