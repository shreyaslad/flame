; startup64.asm

[bits 64]
[extern kmain]

[extern multiboot_header_pointer]
[extern __kernel_end]

section .text
    global _startup64
    _startup64:
        mov rsi, __kernel_end ; Kernel end parameter for the kernel to set up memory
        mov edi, DWORD [multiboot_header_pointer]

        mov eax, 80000000h
        cpuid
        mov maxphysaddr, al
    
        call kmain ; Call the kernel
        
    _halt:
        hlt
        jmp _halt

section .bss
    align 1
    maxphysaddr:
        resb 1