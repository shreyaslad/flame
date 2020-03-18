ENTRY(_start)

OUTPUT_FORMAT(elf64-x86-64)

KNL_HIGH_VMA = 0xFFFFFFFF80000000;

SECTIONS
{
    . = KNL_HIGH_VMA + 0x100000;

    _kernel_start = .;

    .multiboot ALIGN(4K):  AT(ADDR(.multiboot) - KNL_HIGH_VMA)
       {*(.multiboot)}

    .text ALIGN(4K): AT(ADDR(.text) - KNL_HIGH_VMA)
    {*(.text*)}
    .rodata ALIGN(4K): AT(ADDR(.rodata) - KNL_HIGH_VMA)
    {*(.rodata*)}
    .data ALIGN(4K): AT(ADDR(.data) - KNL_HIGH_VMA)
    {*(.data*)}
    .bss ALIGN(4K): AT(ADDR(.bss) - KNL_HIGH_VMA)
    {*(.bss*) *(COMMON)}

    _kernel_end = .;
}