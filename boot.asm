; boot.asm

%define ALIGN (1<<0) ; align loaded modules on page boundaries 
%define MEMINFO (1<<1) ; provide memory map
%define FLAGS (ALIGN | MEMINFO)  ; this is the Multiboot 'flag' field
%define MAGIC 0x1BADB002       ; 'magic number' lets bootloader find the header
%define CHECKSUM -(MAGIC + FLAGS) ; checksum of above, to prove we are multiboot

section .multiboot
	align 4

	dd MAGIC
	dd FLAGS
	dd CHECKSUM

section .data
	gdt:
		dw .gdt_end - .gdt_start - 1
		dq .gdt_start

	  .gdt_start:
		; Null descriptor (required)
	  .null_descriptor:
		dq 0

		; 64 bit mode kernel
	  .kernel_code_64:
		dw 0x0000           ; Limit
		dw 0x0000           ; Base (low 16 bits)
		db 0x00             ; Base (mid 8 bits)
		db 10011010b        ; Access
		db 00100000b        ; Granularity
		db 0x00             ; Base (high 8 bits)

	  .kernel_data:
		dw 0x0000           ; Limit
		dw 0x0000           ; Base (low 16 bits)
		db 0x00             ; Base (mid 8 bits)
		db 10010010b        ; Access
		db 00000000b        ; Granularity
		db 0x00             ; Base (high 8 bits)

	  .gdt_end:

pagemap equ 0x8000
pml4 equ 0x8000
pdpt_low equ 0x9000
pdpt_hi equ 0xa000
pd equ 0xb000
pt equ 0xc000

section .text
	global _start

	_start:
		[bits 32]
		xor ax, ax
		mov ds, ax
		mov es, ax
		mov fs, ax
		mov gs, ax
		mov ss, ax
		mov esp, stack_top

		lgdt [gdt]

		; Multiboot
		mov edi, multiboot_header_pointer
		mov dword [edi], ebx

		; Build the page tables
		xor ax, ax
		mov di, pagemap
		mov cx, (4096 * 5) / 2
		rep stosw

		mov word [pml4], pdpt_low | 0x03
		mov word [pml4 + 511 * 8], pdpt_hi | 0x03
		mov word [pdpt_low], pd   | 0x03
		mov word [pdpt_hi + 511 * 8], pd   | 0x03
		mov word [pd],   pt   | 0x03

		mov eax, 0x03
		mov di, pt
		mov cx, 512
	  
	  .buildpt:
		mov [di], eax
		add eax, 0x1000
		add di, 8
		loop .buildpt

		mov eax, pagemap
		mov cr3, eax

		mov eax, cr4
		or eax, (1 << 5) | (1 << 7)
		mov cr4, eax

		mov ecx, 0xc0000080
		rdmsr
		or eax, 1 << 8
		wrmsr

		mov eax, cr0
		or eax, 1 | (1 << 31)
		mov cr0, eax

		jmp 0x08:.mode64
		
	  [bits 64]
	  .mode64:
		mov ax, 0x10
		mov ds, ax
		mov es, ax
		mov fs, ax
		mov gs, ax
		mov ss, ax

		jmp _startup64

	[bits 64]
	_startup64:
		jmp $

section .bss
	multiboot_header_pointer:
		resb 4

	align 16

	stack_bottom:
		resb 8096
	stack_top: