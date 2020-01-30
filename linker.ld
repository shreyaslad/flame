ENTRY(_start)

SECTIONS
{
    KNL_CORE_START = .;

    .text ALIGN(4K) : AT(ADDR(.text))
    {
        KEEP(*(.multiboot))
        KEEP(*(.text*))
    }

    .rodata ALIGN(4K) : AT(ADDR(.rodata))
    {
        KEEP(*(.rodata*))
    }

    .data ALIGN(4K) : AT(ADDR(.data))
    {
        KEEP(*(.data*))
    }

    .bss ALIGN(4K) : AT(ADDR(.bss))
    {
        KEEP(*(COMMON))
        KEEP(*(.bss))
    }

    KNL_CORE_END = .;
}