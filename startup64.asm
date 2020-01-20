[extern kmain]
[extern KNL_HIGH_VMA]
[extern KNL_CORE_END]

[bits 64]
section .text
    global _startup64

_startup64:
    add rsp, KNL_HIGH_VMA

    mov edx, [KNL_CORE_END]
    mov eax, ebx

    call kmain

_halt:
    hlt
    jmp _halt