; boot.asm
[extern __kernel_start]
[extern __kernel_end]

[extern _startup64]
[extern paging_setup]

%define KNL_HIGH_VMA 0xFFFFFFFF80000000
%define HIGH_VMA 0xFFFF800000000000

ALIGN_MULTIBOOT equ 1<<0
MEMINFO equ 1<<1
FLAGS equ (ALIGN_MULTIBOOT | MEMINFO)
MAGIC equ 0x1BADB002
CHECKSUM equ -(MAGIC + FLAGS)

section .multiboot
    align 4

    dd MAGIC
    dd FLAGS
    dd CHECKSUM

%macro gen_pd_2mb 3
    %assign i %1
    %rep %2
        dq (i | 0x83)
        %assign i i+0x200000
    %endrep
    %rep %3
        dq 0
    %endrep
%endmacro

section .data

    gdt:                           ; Global Descriptor Table (64-bit).
      .null: equ $ - gdt                ; The null descriptor.
        dw 0xFFFF                       ; Limit (low).
        dw 0                            ; Base (low).
        db 0                            ; Base (middle)
        db 0                            ; Access.
        db 0                            ; Granularity.
        db 0                            ; Base (high).
      .code: equ $ - gdt                ; The code descriptor.
        dw 0                            ; Limit (low).
        dw 0                            ; Base (low).
        db 0                            ; Base (middle)
        db 10011010b                    ; Access (exec/read).
        db 10101111b                    ; Granularity, 64 bits flag, limit19:16.
        db 0                            ; Base (high).
      .data: equ $ - gdt                ; The data descriptor.
        dw 0                            ; Limit (low).
        dw 0                            ; Base (low).
        db 0                            ; Base (middle)
        db 10010010b                    ; Access (read/write).
        db 00000000b                    ; Granularity.
        db 0                            ; Base (high).

      .pointer:                         ; The GDT-pointer.
        dw $ - gdt - 1                  ; Limit.
        dq gdt                          ; Base.
      .pointer32:                       ; The GDT-pointer for 32 bit mode.
        dw $ - gdt - 1                  ; Limit.
        dd gdt - KNL_HIGH_VMA           ; Base.

section .bss
    align 16

    global multiboot_header_pointer
    multiboot_header_pointer:
        resb 16

    stack_bottom:
        resb 65536
    stack_top:

section .data
    align 4096
    pml4t:
        dq (pml3 - KNL_HIGH_VMA) + 0x3
        times 255 dq 0
        dq (pml3 - KNL_HIGH_VMA) + 0x3
        times 254 dq 0
        dq (pml3 - KNL_HIGH_VMA) + 0x3
    pml3:
        dq (pml2 - KNL_HIGH_VMA) + 0x3
        times 509 dq 0
        dq (pml2 - KNL_HIGH_VMA) + 0x3
        dq 0
    pml2:
        gen_pd_2mb 0, 512, 0

section .text
    [bits 32]

    global _start
    _start:
        mov edi, multiboot_header_pointer - KNL_HIGH_VMA
        mov DWORD [edi], ebx
        mov eax, pml4t - KNL_HIGH_VMA
        mov cr3, eax
        ; Paging

        mov eax, cr4                 ; Set the A-register to control register 4.
        or eax, 1 << 5               ; Set the PAE-bit, which is the 6th bit (bit 5).
        mov cr4, eax                 ; Set control register 4 to the A-register.

        ; Switch to long mode
        mov ecx, 0xC0000080          ; Set the C-register to 0xC0000080, which is the EFER MSR.
        rdmsr                        ; Read from the model-specific register.
        or eax, 1 << 8               ; Set the LM-bit which is the 9th bit (bit 8).
        wrmsr                        ; Write to the model-specific register.
        mov eax, cr0                 ; Set the A-register to control register 0.
        or eax, 1 << 31              ; Set the PG-bit, which is the 32nd bit (bit 31).
        mov cr0, eax                 ; Set control register 0 to the A-register.

        ; Set up GDT
        lgdt [gdt.pointer32 - KNL_HIGH_VMA]
        jmp gdt.code:loaded - KNL_HIGH_VMA

    [bits 64]

    loaded:
        lgdt [gdt.pointer]          ; Load the 64-bit global descriptor table.
        mov ax, gdt.data            ; Set the A-register to the data descriptor.
        mov ds, ax                  ; Set the data segment to the A-register.
        mov es, ax                  ; Set the extra segment to the A-register.
        mov fs, ax                  ; Set the F-segment to the A-register.
        mov gs, ax                  ; Set the G-segment to the A-register.
        mov ss, ax                  ; Set the stack segment to the A-register.
        mov rsp, stack_top          ; Set the stack up

        ; long jump
        mov rax, _startup64
        jmp rax