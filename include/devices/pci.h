/*
    pci.h
    Copyright Shreyas Lad (PenetratingShot) 2020

    PCI Bus Driver
*/

#pragma once

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <sys/ports.h>

#define PCI_CONFIG_ADDRESS 0xCF8
#define PCI_CONFIG_DATA 0xCFC

/* General */
#define HEADER_OFFSET 0x0E
#define CAPABILITY_POINTER_OFFSET 0x38

typedef struct {
    uint32_t bus;
    uint32_t slot;
    uint32_t func;
} device_t;

device_t* createDevice(uint32_t bus, uint32_t slot, uint32_t func);
uint32_t createAddress(device_t* device, uint32_t offset);

uint32_t pciPIOConfigRead(device_t* device, uint32_t offset);
void pciPIOConfigWrite(device_t* device, uint32_t offset, uint32_t data);