[extern _startup64]
[extern KNL_HIGH_VMA]

CODE_USER equ 0x40C3FA000000D090 
DATA_USER equ 0x40C3F2000000D090 
CODE_KERNEL equ 0x00CF9A000000FFFF 
DATA_KERNEL equ 0x00CF92000000FFFF 

%assign ALIGN    1<<0             ; align loaded modules on page boundaries 
%assign MEMINFO  1<<1             ; provide memory map
%assign FLAGS    ALIGN | MEMINFO  ; this is the Multiboot 'flag' field
%assign MAGIC    0x1BADB002       ; 'magic number' lets bootloader find the header
%assign CHECKSUM -(MAGIC + FLAGS) ; checksum of above, to prove we are multiboot

section .multiboot
	align 4
	dd MAGIC
	dd FLAGS
	dd CHECKSUM

section .data
	align 16

	global gdt64
	gdt64:
	    dq 0
		dq CODE_KERNEL
		dq DATA_KERNEL
		dq CODE_USER
		dq DATA_USER
		
	align 16

	global gdt_ptr
	gdt_ptr:
		dw $ - gdt64 - 1
		.addr: dq gdt64

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

section data
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
	align 4
	global reset_seg
	reset_seg:
		mov ax, 0x10
		mov ds, ax
		mov es, ax
		mov fs, ax
		mov gs, ax
		mov ss, ax

section .text
	align 4
	global data_seg
	data_seg:
		mov ax, 0x0
		mov ds, ax
		mov es, ax
		mov fs, ax
		mov gs, ax
		mov ss, ax

section .text
	[bits 32]
	global _start

	_start:
		cli
		mov esp, stack_top

		mov eax, gdt64
		sub eax, _KNL_HIGH_VMA
		mov dword [gdt_ptr.addr], eax

		lgdt [gdt_ptr] - KNL_HIGH_VMA

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
		mov cr0, eax		

		;jmp 0x08:_mode64 - KNL_HIGH_VMA
		mov eax, _mode64
		push eax
		push 0x08
		retf

	[bits 64]
	_mode64:
		mov rax, _higher_half
		jmp rax

	_higher_half:
		mov qword [gdt_ptr.addr], rax
		lgdt [gdt_ptr]
		jmp _startup64