.set ALIGN,    1<<0             /* align loaded modules on page boundaries */
.set MEMINFO,  1<<1             /* provide memory map */
.set FLAGS,    ALIGN | MEMINFO  /* this is the Multiboot 'flag' field */
.set MAGIC,    0x1BADB002       /* 'magic number' lets bootloader find the header */
.set CHECKSUM, -(MAGIC + FLAGS) /* checksum of above, to prove we are multiboot */

.section .multiboot, "a"
	.align 4
	.long MAGIC
	.long FLAGS
	.long CHECKSUM
 
.section .rodata
    .equ CODE_USER,        0x40C3FA000000D090 
    .equ DATA_USER,        0x40C3F2000000D090 
    .equ CODE_KERNEL,      0x00CF9A000000FFFF 
    .equ DATA_KERNEL,      0x00CF92000000FFFF 
	.align 16
	gdt64:
	    .quad 0
		.quad CODE_KERNEL
		.quad DATA_KERNEL
		.quad CODE_USER
		.quad DATA_USER
	.align 16
	.global gdt_ptr
	gdt_ptr:
		.word . - gdt64 - 1
	    .quad (gdt64)

.altmacro
.macro gen_pd_2mb a b c
    .set i, \a
	.rept \b
		.quad (i | 0x83)
		.set i, i + 0x200000
	.endr
	.rept \c
		.quad 0
    .endr
.endm

.section .data
    .align 4096
    boot_pml4:
	    .quad boot_pml3_1 + 3
		.fill 255,8,0
		.quad boot_pml3_phys + 3
		.fill 254,8,0
		.quad boot_pml3_2 + 3
	boot_pml3_1:
		.quad boot_pml2 + 3
		.fill 511,8,0
	boot_pml3_2:
		.fill 510,8,0
		.quad boot_pml2 + 3
		.fill 1,8,0
	boot_pml3_phys:
		.quad boot_pml2_phys + 3
		.fill 511,8,0
	boot_pml2:
		gen_pd_2mb 0, 64, 0
		.fill 448,8,0
	boot_pml2_phys:
		gen_pd_2mb 0, 512, 0  

.section .bss
	.align 16
	stack_bottom:
		.skip 65536
	stack_top:

.section .text
	.align 4
	.global reset_seg
	reset_seg:
		mov $0x10, %ax
		mov %ax, %ds
		mov %ax, %es
		mov %ax, %fs
		mov %ax, %gs
		mov %ax, %ss

.section .text
	.align 4
	.global data_seg
	data_seg:
		mov $0x0, %ax
		mov %ax, %ds
		mov %ax, %es
		mov %ax, %fs
		mov %ax, %gs
		mov %ax, %ss

/*
The linker script specifies _start as the entry point to the kernel and the
bootloader will jump to this position once the kernel has been loaded. It
doesn't make sense to return from this function as the bootloader is gone.
*/
.section .text
	.code32
	.global _start
	.type _start, @function
	_start:
		cli
		mov $stack_top, %esp

		lgdt (gdt_ptr)

		mov %eax, %edi
		mov %ebx, %esi

		mov %cr4, %eax
		or $0x000000A0, %eax
		mov %eax, %cr4

		mov $boot_pml4, %eax
		mov %eax, %cr3

		mov $0xC0000080, %ecx
		rdmsr
		or $0x00000901, %eax
		wrmsr

		mov %cr0, %eax
		or $(1 << 31), %eax
		mov %eax, %cr0
		
		jmp $0x08, $higher_half_entry

	.code64
	higher_half_entry:
		mov $higher_half, %rax
		jmp %rax

	higher_half:
		lgdt (gdt_ptr)
		jmp startup64