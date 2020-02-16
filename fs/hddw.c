/*
                hddw.c
                Copyright Menotdan 2019
                Copyright Shreyas Lad (PenetratingShot) 2020

                Virtual File System
*/

#include <fs/hddw.h>

uint8_t* readBuffer;
uint8_t* writeBuffer;
uint16_t readOut[256];
uint16_t writeIn[256];
uint16_t emptySector[256];
uint32_t driveUseTick;
uint32_t lastSector = 0;
uint8_t current_drive = 1;

void init_hddw() {
  for (int i = 0; i < 256; i++) {
    emptySector[i] = 0x0;
  }
  readBuffer = (uint8_t*)malloc(512);
  writeBuffer = (uint8_t*)malloc(512);
}

void select_drive(uint8_t driveToSet) {
  if (driveToSet == 1) {
    if (hdd.mp == 0) {
      ata_drive = MASTER_DRIVE;
      ata_controller = PRIMARY_IDE;
      hdd.nodrives = 0;
      current_drive = driveToSet;
    } else if (hdd.mp48 == 0) {
      ata_drive = MASTER_DRIVE_PIO48;
      ata_controller = PRIMARY_IDE;
      ata_pio = 1;
      hdd.nodrives = 0;
      current_drive = driveToSet;
    } else {
      kprint("That drive is offline!\n");
    }
  } else if (driveToSet == 2) {
    if (hdd.ms == 0) {
      ata_drive = SLAVE_DRIVE;
      ata_controller = PRIMARY_IDE;
      hdd.nodrives = 0;
      current_drive = driveToSet;
    } else if (hdd.ms48 == 0) {
      ata_drive = SLAVE_DRIVE_PIO48;
      ata_controller = PRIMARY_IDE;
      ata_pio = 1;
      hdd.nodrives = 0;
      current_drive = driveToSet;
    } else {
      kprint("That drive is offline!\n");
    }
  } else if (driveToSet == 3) {
    if (hdd.sp == 0) {
      ata_drive = MASTER_DRIVE;
      ata_controller = SECONDARY_IDE;
      hdd.nodrives = 0;
      current_drive = driveToSet;
    } else if (hdd.sp48 == 0) {
      ata_drive = MASTER_DRIVE_PIO48;
      ata_controller = SECONDARY_IDE;
      ata_pio = 1;
      hdd.nodrives = 0;
      current_drive = driveToSet;
    } else {
      kprint("That drive is offline!\n");
    }
  } else if (driveToSet == 4) {
    if (hdd.ss == 0) {
      ata_drive = SLAVE_DRIVE;
      ata_controller = SECONDARY_IDE;
      ata_pio = 0;
      hdd.nodrives = 0;
      current_drive = driveToSet;
    } else if (hdd.ss48 == 0) {
      ata_drive = SLAVE_DRIVE_PIO48;
      ata_controller = SECONDARY_IDE;
      ata_pio = 1;
      hdd.nodrives = 0;
      current_drive = driveToSet;
    } else {
      kprint("That drive is offline!\n");
    }
  } else {
    kprint("Not a valid drive!\n");
  }
}

void read(uint32_t sector, uint32_t sector_high) {
  if (sector_high == 1) {
  }
  if (ata_pio == 0) {
    ata_pio28(ata_controller, 1, ata_drive,
              sector); // Read disk into ata_buffer
  } else {
    ata_pio48(ata_controller, 1, ata_drive,
              sector); // Read disk into ata_buffer
  }
  bool f = false;
  for (int i = 0; i < 256; i++) {
    if (ata_buffer[i] != 0) {
      f = true;
    }
    readOut[i] = ata_buffer[i];
  }
  clear_ata_buffer();
  if (f == false &&
      (tick - driveUseTick > 5000 || abs(sector - lastSector) > 50)) {
    // Sometimes the drive shuts off, so we need to wait for it to turn on
    wait(2000);
    if (ata_pio == 0) {
      ata_pio28(ata_controller, 1, ata_drive,
                sector); // Read disk into ata_buffer
    } else {
      ata_pio48(ata_controller, 1, ata_drive,
                sector); // Read disk into ata_buffer
    }

    for (int i = 0; i < 256; i++) {
      readOut[i] = ata_buffer[i];
    }
  }
  driveUseTick = tick;
  lastSector = sector;
}

void readToBuffer(uint32_t sector) {
  uint8_t* ptr = readBuffer;
  read(sector, 0);

  for (uint32_t i = 0; i < 256; i++) {
    uint16_t in = readOut[i];
    uint8_t s = (uint8_t)(in >> 8);   // Default is f
    uint8_t f = (uint8_t)(in & 0xff); // Default is s

    *ptr = s;
    ptr++;
    
    *ptr = f;
    ptr++;
  }

}

void read_disk(uint32_t sector) {
  char str2[32];
  kprint("\nSector ");
  kprint_int(sector);
  kprint(" contents:\n\n");

  //! read sector from disk
  readToBuffer(sector);
  uint8_t* temp = readBuffer;
  for (int l = 0; l < 256; l++) {
    uint16_t good = *temp;
    temp++;
    good += (uint16_t)(*temp << 8);
    temp++;
    hex_to_ascii(good, str2);
    kprint(str2);
    kprint(" ");
    for (int i = 0; i < 32; i++) {
      str2[i] = 0;
    }
  }
  clear_ata_buffer();
}

void writeFromBuffer(uint32_t sector, uint8_t badcheck) {
  uint8_t* ptr = writeBuffer;
  for (uint32_t i = 0; i < 256; i++) {
    uint8_t f = *ptr; // Default is s
    ptr++;
    uint8_t s = *ptr; // Default is f
    ptr++;
    uint16_t wd;
    wd = ((uint16_t)f << 8) | s;

    writeIn[i] = wd;
  }
  write(sector, badcheck);
}

void copy_sector(uint32_t sector1, uint32_t sector2) {
  ata_pio28(ata_controller, 1, ata_drive, sector1);
  ata_pio28(ata_controller, 2, ata_drive, sector2);
  clear_ata_buffer();
}

void write(uint32_t sector, uint8_t badcheck) {
  if ((tick - driveUseTick > 5000 || abs(sector - lastSector) > 50)) {
    read(sector, 0); // Start the drive
  }
  for (int i = 0; i < 256; i++) {
    ata_buffer[i] = writeIn[i];
  }
  if (ata_pio == 0) {
    ata_pio28(ata_controller, 2, ata_drive, sector);
  } else {
    ata_pio48(ata_controller, 2, ata_drive, sector);
  }
  uint32_t bad = 1;
  while (bad != 0) {
    if (ata_pio == 0) {
      ata_pio28(ata_controller, 2, ata_drive, sector);
    } else {
      ata_pio48(ata_controller, 2, ata_drive, sector);
    }
    read(sector, 0);
    bad = 0;
    for (int i = 0; i < 256; i++) {
      if (writeIn[i] != readOut[i]) {
        if (badcheck == 1) {
          bad++;
        }
      }
    }
    if (badcheck == 1) {
      sprint("\n");
      sprint_uint(bad);
    }
  }
  clear_ata_buffer();
  for (int i = 0; i < 256; i++) {
    writeIn[i] = emptySector[i];
  }
  lastSector = sector;
}

void clear_sector(uint32_t sector) {
  for (int i = 0; i < 256; i++) {
    ata_buffer[i] = emptySector[i];
  }
  if (ata_pio == 0) {
    ata_pio28(ata_controller, 2, ata_drive, sector);
  } else {
    ata_pio48(ata_controller, 2, ata_drive, sector);
  }
}