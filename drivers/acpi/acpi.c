/*
    acpi.c
    Copyright Shreyas Lad (PenetratingShot) 2020

    Advanced Configuration Power Interface
*/

#include <drivers/acpi/acpi.h>

rsdp_t* rsdp = NULL;
rsdt_t* rsdt = NULL;
xsdt_t* xsdt = NULL;

void initACPI() {
    for (size_t i = 0x80000 + HIGH_VMA; i < 0x100000 + HIGH_VMA; i += 16) {
        if (i == 0xa0000 + HIGH_VMA) {
            /* skip video mem and mapped hardware */
            i = 0xe0000 - 16 + HIGH_VMA;
            continue;
        }
        if (!strncmp((char *)i, "RSD PTR ", 8)) {
            printf("acpi: Found RSDP at %x\n", i);
            rsdp = (rsdp_t*)i;
            goto rsdp_found;
        }
    }

    panic("Non ACPI Compliant System");
    return;

    sprintf("RSDP Revision: %d\n", rsdp->revision);

    rsdp_found:
        if (rsdp->revision == 2 && rsdp->xsdtAddr) {
            printf("acpi: Found XSDT at %x\n", ((size_t)rsdp->xsdtAddr + HIGH_VMA));

            xsdt = (xsdt_t*)((size_t)rsdp->xsdtAddr + HIGH_VMA);
        } else {
            printf("acpi: Found RSDT at %x\n", ((size_t)rsdp->rsdtAddr + HIGH_VMA));

            rsdt = (rsdt_t*)((size_t)rsdp->rsdtAddr + HIGH_VMA);
            sprintf("RSDT Addr: %x\nRSDP Virt Addr: %x\n", (size_t)rsdp->rsdtAddr, (size_t)rsdp->rsdtAddr + HIGH_VMA);
            sprintf("RDST SDT: %x\n", (uint64_t)rsdt->sdtPtr);
        }

        return;
}

void* find_sdt(const char* signature, uint64_t index) {
    sdt_t* ptr = NULL;
    uint64_t count = 0;
    
    if (xsdt != NULL) {
        // if the xsdt was previously found, try to find the given sdt
        // this could be either the FADT or the DSDT
        // all SDTs have a character signature that exists in memory
        for (uint64_t i = 0;i < (xsdt->sdt.length - sizeof(sdt_t)) / 8; i++) {
            ptr = (xsdt_t*)((size_t)xsdt->sdtPtr[i] + HIGH_VMA);
            if (!strncmp(ptr->signature, signature, 4)) {
                if (count++ == index) {
                    printf("acpi: Found \"%s\" at %x\n", signature, (uint64_t)ptr);
                    return ptr;
                }
            }
        }
    } else {
        // if the xdst was previously found, we can assume that the rsdt was
        // do the same thing but with the rsdt

        for (uint64_t i = 0; i < (rsdt->sdt.length - sizeof(sdt_t)) / 8; i++) {
            sprintf("max: %d\n", (rsdt->sdt.length - sizeof(sdt_t)) / 8);
            sprintf("%x\n", (uint64_t)rsdt);
            /*ptr = (sdt_t*)((size_t)rsdt->sdtPtr[i] + HIGH_VMA);
            if (!strncmp(ptr->signature, signature, 4)) {
                if (count++ == index) {
                    sprintf("acpi: Found \"%s\" at %x\n", signature, (uint64_t)ptr);
                    return ptr;
                }
            }*/
        }
    }

    sprintf("acpi: \"%s\" not found\n", (char*)signature);
    return NULL;
}