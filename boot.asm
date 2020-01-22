[extern _startup64]

%define KNL_HIGH_VMA 0xffffffff80000000
CODE_USER equ 0x40C3FA000000D090 
DATA_USER equ 0x40C3F2000000D090 
CODE_KERNEL equ 0x00CF9A000000FFFF 
DATA_KERNEL equ 0x00CF92000000FFFF 

%define ALIGN (1<<0) ; align loaded modules on page boundaries 
%define MEMINFO (1<<1) ; provide memory map
%define FLAGS (ALIGN | MEMINFO)  ; this is the Multiboot 'flag' field
%define MAGIC 0x1BADB002       ; 'magic number' lets bootloader find the header
%define CHECKSUM -(MAGIC + FLAGS) ; checksum of above, to prove we are multiboot

%macro gen_pd_2mb 3
	%assign i %1
	%rep %2
		dq (i | 0x83)
		%assign i i + 0x200000
	%endrep
	%rep %3
		dq 0
	%endrep
%endmacro

section .multiboot
	align 4
	dd MAGIC
	dd FLAGS
	dd CHECKSUM

section .data
	align 16
	gdt:
	    dq 0
		dq CODE_KERNEL
		dq DATA_KERNEL
		dq CODE_USER
		dq DATA_USER
		
	align 16
	global gdt_ptr
	gdt_ptr:
		dw $ - gdt - 1
		.addr: dq 0

    align 4096
    boot_pml4:
	    dq boot_pml3_1 + 3
		times 255 dq 0
		dq boot_pml3_phys + 3
		times 254 dq 0
		dq boot_pml3_2 + 3
	boot_pml3_1:
		dq boot_pml2 + 3
		times 511 dq 0
	boot_pml3_2:
		times 510 dq 0
		dq boot_pml2 + 3
		dq 0
	boot_pml3_phys:
		dq boot_pml2_phys + 3
		times 511 dq 0
	boot_pml2:
		gen_pd_2mb 0, 64, 0
		times 448 dq 0
	boot_pml2_phys:
		gen_pd_2mb 0, 512, 0

section .bss
	align 16
	stack_bottom:
		resb 65536
	stack_top:

section .text
	[bits 32]
	global _start

	_start:
		cli
		mov esp, stack_top - KNL_HIGH_VMA

		mov eax, gdt - KNL_HIGH_VMA
		mov dword [gdt_ptr.addr - KNL_HIGH_VMA], eax

		lgdt [gdt_ptr - KNL_HIGH_VMA]

		mov edi, eax
		mov esi, ebx

		mov eax, cr4
		or eax, 0x000000A0
		mov cr4, eax

		mov eax, boot_pml4 - KNL_HIGH_VMA
		mov cr3, eax

		mov ecx, 0xC0000080
		rdmsr
		or eax, 0x00000901
		wrmsr

		mov eax, cr0
		or eax, (1 << 31)

		; identity map the tables
		mov eax, 0x03 ; all entires have to be 0x03 for present+writeable
		mov edi, KNL_HIGH_VMA

		; loop counter
    	; every entry covers 4kb, 4x512 = 2048 = 2MB
		mov cx, 512 ; only map first 2 MB

		_buildpt:
			mov [edi], eax ; write eax to the current entry
			add eax, 0x1000 ; increase eax by 4kb so the next entry is also identity mapped
			add edi, 8 ; increase current entry pointer
			loop _buildpt ; decrease ecx, compare to 0, then loop if ne

		mov cr0, eax

		jmp 0x08:_mode64 - KNL_HIGH_VMA

	[bits 64]
	_mode64:
		mov rax, _higher_half
		jmp rax

	_higher_half:
		mov qword [gdt_ptr.addr], gdt
		lgdt [gdt_ptr]
		jmp _startup64
