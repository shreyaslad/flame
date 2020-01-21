ENTRY(_start)

SECTIONS
{
    KNL_HIGH_VMA = 0xFFFFFFFFC0000000;
    . = KNL_HIGH_VMA + 1M;

    .text ALIGN(4K) : AT(ADDR(.text) - KNL_HIGH_VMA)
    {
        KEEP(*(.multiboot))
        KEEP(*(.text*))
    }

    .rodata ALIGN(4K) : AT(ADDR(.rodata) - KNL_HIGH_VMA)
    {
        KEEP(*(.rodata*))
    }

    .data ALIGN(4K) : AT(ADDR(.data) - KNL_HIGH_VMA)
    {
        KEEP(*(.data*))
    }

    .bss ALIGN(4K) : AT(ADDR(.bss) - KNL_HIGH_VMA)
    {
        KEEP(*(COMMON))
        KEEP(*(.bss))
    }

    KNL_CORE_END = 0x0;
}