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
    uint8_t class;
    uint8_t subclass;
    uint8_t progif;
} id_t;

typedef struct {
    uint8_t bus;
    uint8_t slot;
    uint8_t func;
} device_t;

device_t* createDevice(uint8_t bus, uint8_t slot, uint8_t func);
uint32_t createAddress(uint8_t bus, uint8_t slot, uint8_t function, uint16_t offset);

uint8_t pciPIOConfigReadB(uint8_t bus, uint8_t slot, uint8_t function, uint16_t offset);
void pciPIOConfigWriteB(uint8_t bus, uint8_t slot, uint8_t function, uint16_t offset, uint8_t data);

uint16_t pciPIOConfigReadW(uint8_t bus, uint8_t slot, uint8_t function, uint16_t offset);
void pciPIOConfigWriteW(uint8_t bus, uint8_t slot, uint8_t function, uint16_t offset, uint16_t data);

uint32_t pciPIOConfigReadD(uint8_t bus, uint8_t slot, uint8_t function, uint16_t offset);
void pciPIOConfigWriteD(uint8_t bus, uint8_t slot, uint8_t function, uint16_t offset, uint32_t data);