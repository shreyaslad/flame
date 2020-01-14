/* The bootloader will look at this image and start execution at the symbol
   designated as the entry point. */
ENTRY(_start)

OUTPUT_FORMAT(elf64-x86-64)

KNL_HIGH_VMA = 0xFFFFFFFF80000000;
KNL_BOOT_VMA = 0x100000;

/* Tell where the various sections of the object files will be put in the final
   kernel image. */
SECTIONS
{
	. = KNL_BOOT_VMA;
	.multiboot ALIGN(4K): 
	{
		*(.multiboot)
  }

  .bootstrap ALIGN(4K): 
  {
    	boot.o *(.text)
    	boot.o *(.bss)
		  boot.o *(COMMON)
		  boot.o *(.rodata)
    	boot.o *(.data)
  }

  . += KNL_HIGH_VMA;

  .text ALIGN(4K): AT(ADDR(.text) - KNL_HIGH_VMA)
  {
    EXCLUDE_FILE (boot.o) *(.text)
  }

  .data ALIGN(4K): AT(ADDR(.data) - KNL_HIGH_VMA)
  {
    EXCLUDE_FILE (boot.o) *(.data)
	  EXCLUDE_FILE (boot.o) *(.rodata)
  }

  .bss ALIGN(4K): AT(ADDR(.bss) - KNL_HIGH_VMA)
  {
    EXCLUDE_FILE (boot.o) *(COMMON)
	  EXCLUDE_FILE (boot.o) *(.bss)
  }
   KNL_CORE_END = .;
}