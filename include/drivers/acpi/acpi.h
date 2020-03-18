/*
    acpi.h
    Copyright Shreyas Lad (PenetratingShot) 2020

    Advanced Configuration Power Interface
*/

#pragma once

#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <mm/mem.h>

/*
    Although these are defined in 
    acpispec/tables.h, the impl is
    different from that header file
*/

typedef struct {
    char signature[8];
    uint8_t checksum;
    char OEMID[6];
    uint8_t revision;
    uint32_t rsdtAddr;

    /* V2 Only */
    uint32_t length;
    uint64_t xsdtAddr;
    uint8_t extChecksum;
    uint8_t reserved[3];
} __attribute__((packed)) rsdp_t;

typedef struct {
    char signature[4];
    uint32_t length;
    uint8_t revision;
    uint8_t checksum;
    char OEMID[6];
    char OEMTABLEID[8];
    uint32_t oemRevision;
    uint32_t creatorID;
    uint32_t creatorRevision;
} __attribute__((packed)) sdt_t;

typedef struct {
    sdt_t sdt;
    uint32_t* sdtPtr;
} __attribute__((packed)) rsdt_t;

typedef struct {
    sdt_t sdt;
    uint64_t* sdtPtr;
} __attribute__((packed)) xsdt_t;

rsdp_t* rsdp;
rsdt_t* rsdt;
xsdt_t* xsdt;

void initACPI();