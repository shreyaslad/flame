/*
    laihost.c
    Copyright Shreyas Lad (PenetratingShot) 2020

    LAI Impls
*/

#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/ports.h>
#include <sys/timer.h>
#include <mm/mem.h>
#include <mm/vmm.h>
#include <drivers/pci.h>
#include <acpispec/tables.h>

void laihost_log(int level, const char* str) {
    UNUSED(level);

    sprintf((char*)str);
    sprintf("\n");
}

void laihost_panic(const char* str) {
    panic((char*)str);
}

void* laihost_malloc(size_t size) {
    return malloc(size);
}

void laihost_realloc(void* ptr, size_t size) {
    return realloc(ptr, size);
}

void laihost_free(void* p) {
    free(p);
}

void* laihost_scan(const char* signature, size_t index) {
    // the DSDT must be found using a pointer in the FADT
    if (!strncmp(signature, "DSDT", 4)) {
        if (index > 0) {
            panic("0 is the only valid DSDT index");
            return;
        }

        acpi_fadt_t* fadt = (acpi_fadt_t*)find_sdt("FACP", 0);
        void* dsdt = (char*)(size_t)fadt->dsdt + HIGH_VMA;
        printf("apci: DSDT Addr is %x\n", (uint64_t)dsdt);

        return dsdt;
    } else {
        return find_sdt(signature, index);
    }
}

uint8_t laihost_inb(uint16_t port) {
    return inb(port);
}

void laihost_outb(uint16_t port, uint8_t value) {
    outb(port, value);
}

uint16_t laihost_inw(uint16_t port) {
    return inw(port);
}

void laihost_outw(uint16_t port, uint16_t value) {
    outw(port, value);
}

uint32_t laihost_ind(uint16_t port) {
    return ind(port);
}

void laihost_outd(uint16_t port, uint32_t value) {
    outd(port, value);
}

void laihost_sleep(uint64_t duration) {
    wait(duration);
}

void laihost_pci_writeb(uint16_t segment, uint8_t bus, uint8_t slot, uint8_t function, uint16_t offset, uint8_t value) {
    if (segment) panic("PCI segments not yet supported");
    
    pciPIOConfigWriteB(bus, slot, function, offset, value);
}

uint8_t laihost_pci_readb(uint8_t segment, uint8_t bus, uint8_t slot, uint8_t function, uint16_t offset) {
    if (segment) panic("PCI segments not yet supported");
    
    return pciPIOConfigReadB(bus, slot, function, offset);
}

void laihost_pci_writew(uint16_t segment, uint8_t bus, uint8_t slot, uint8_t function, uint16_t offset, uint16_t value) {
    if (segment) panic("PCI segments not yet supported");
    
    pciPIOConfigWriteW(bus, slot, function, offset, value);
}

uint16_t laihost_pci_readw(uint8_t segment, uint8_t bus, uint8_t slot, uint8_t function, uint16_t offset) {
    if (segment) panic("PCI segments not yet supported");
    
    return pciPIOConfigReadW(bus, slot, function, offset);
}

void laihost_pci_writed(uint16_t segment, uint8_t bus, uint8_t slot, uint8_t function, uint16_t offset, uint32_t value) {
    if (segment) panic("PCI segments not yet supported");
    
    pciPIOConfigWriteD(bus, slot, function, offset, value);
}

uint32_t laihost_pci_readd(uint8_t segment, uint8_t bus, uint8_t slot, uint8_t function, uint16_t offset) {
    if (segment) panic("PCI segments not yet supported");
    
    return pciPIOConfigReadD(bus, slot, function, offset);
}

void* laihost_map(size_t physaddr, size_t count) {
    UNUSED(count);

    return physaddr + HIGH_VMA;
}

void laihot_unmap(size_t physaddr, size_t count) {
    ; //bruh
}