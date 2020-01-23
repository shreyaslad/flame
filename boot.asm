[extern _startup64]

%define KNL_HIGH_VMA 0xFFFFFFFFC0000000

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
		dw .gdt_end - .gdt_start - 1
		dq .gdt_start

	  .gdt_start:

	  .null:
		dq 0

	  .code64:
		dw 0x0000 ; Limit
		dw 0x0000 ; Base (low 16)
		db 0x00 ; Base (mid 8)
		db 10011010b ; Access
		db 00100000b ; Granularity
		db 0x00 ; Base (high 8)

	  .data64:
		dw 0x0000 ; Limit
		dw 0x0000 ; Base (low 16)
		db 0x00 ; Base (mid 8)
		db 10010010b ; Access
		db 00000000b ; Granularity
		db 0x00 ; Base (high 8)

	  .gdt_end:

		
	align 16
	global gdt_ptr
	gdt_ptr:
		dw $ - gdt - 1
	  .addr: dq 0

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

		mov eax, PML4 - KNL_HIGH_VMA
		mov cr3, eax

		mov ecx, 0xC0000080
		rdmsr
		or eax, 0x00000901
		wrmsr

	  .pt_init:
		mov eax, PDP - KNL_HIGH_VMA
		or ax, 0b11
		mov [PML4 - KNL_HIGH_VMA], eax

		mov eax, PD - KNL_HIGH_VMA
		or ax, 0xb11
		mov [PDP - KNL_HIGH_VMA], eax

		mov eax, PT - KNL_HIGH_VMA
		or ax, 0b11
		mov [PD - KNL_HIGH_VMA], eax

		mov eax, 0x03
		mov edi, PT - KNL_HIGH_VMA

		mov cx, 512

	  .pt_build:
		mov [edi], eax
		add eax, 0x1000
		add edi, 8
		
		loop _pt_build

		mov eax, cr0
		or eax, (1 << 31)
		mov edi, KNL_HIGH_VMA

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


section .bss
	align 16

	stack_bottom:
		resb 65536
	stack_top:

	align 4096

	PML4: resb 4096
	PDP: resb 4096
	PD: resb 4096
	PT: resb 4096