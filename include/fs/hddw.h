/*
    hddw.c
    Copyright Menotdan 2019
    Copyright Shreyas Lad (PenetratingShot) 2020

    ATA Drive I/O
*/

#pragma once

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <devices/time.h>
#include <devices/screen/serial.h>
#include <devices/screen/screen.h>
#include <fs/hdd.h>

uint16_t readOut[256];
uint16_t writeIn[256];
uint8_t *readBuffer;
uint8_t *writeBuffer;
uint8_t current_drive;

void read(uint32_t sector, uint32_t sector_high);
void copy_sector(uint32_t sector1, uint32_t sector2);
void readToBuffer(uint32_t sector);
void writeFromBuffer(uint32_t sector, uint8_t badcheck);
void write(uint32_t sector, uint8_t badcheck);
void init_hddw();
void clear_sector(uint32_t sector);
void select_drive(uint8_t driveToSet);

void read_disk(uint32_t sector);