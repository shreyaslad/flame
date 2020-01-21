ENTRY(_start)

_KNL_HIGH_VMA = 0xFFFFFFFFC0000000;

SECTIONS
{
    _KNL_HIGH_VMA = 0xFFFFFFFFC0000000;
    . = _KNL_HIGH_VMA + 1M;

    .text ALIGN(4K) : AT(ADDR(.text) - _KNL_HIGH_VMA)
    {
        KEEP(*(.multiboot))
        KEEP(*(.text*))
    }

    .rodata ALIGN(4K) : AT(ADDR(.rodata) - _KNL_HIGH_VMA)
    {
        KNL_HIGH_VMA = _KNL_HIGH_VMA;
        KEEP(*(.rodata*))
    }

    .data ALIGN(4K) : AT(ADDR(.data) - _KNL_HIGH_VMA)
    {
        KEEP(*(.data*))
    }

    .bss ALIGN(4K) : AT(ADDR(.bss) - _KNL_HIGH_VMA)
    {
        KEEP(*(COMMON))
        KEEP(*(.bss))

        KNL_CORE_END = 0x0;
    }
}