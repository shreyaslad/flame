; startup64.asm

[bits 64]
[extern kmain]

[extern multiboot_header_pointer]
[extern __kernel_end]

section .text
    global _startup64
    _startup64:
        mov edi, DWORD [multiboot_header_pointer]
    
        call kmain ; Call the kernel
        
    _halt:
        hlt
        jmp _halt