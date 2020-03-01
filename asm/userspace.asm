; userspace.asm
; Copyright Shreyas Lad (PenetratingShot) 2020
;
; Setup the userspace

[extern user_stack_top]
[extern KNL_HIGH_VMA]
[extern gdt]

; this macro is repeated why
%macro pushaq 0
    push rax
    push rbx
    push rcx
    push rdx
    push rbp
    push rsi
    push rdi
    push r8
    push r9
    push r10
    push r11
    push r12
    push r13
    push r14
    push r15
%endmacro

%macro popaq 0
    pop r15
    pop r14
    pop r13
    pop r12
    pop r11
    pop r10
    pop r9
    pop r8
    pop rdi
    pop rsi
    pop rbp
    pop rdx
    pop rcx
    pop rbx
    pop rax
%endmacro

; usermode switch
%macro pushcpu 1
    push 0x24
    push 0 ; SS
    push 0 ; RFLAGS
    push rip ; need to push the actual address for the userspace code here instead of the rip
    push DEADBEEFh
    iret
%endmacro

global _init_userspace
; rax will contain the userspace instruction
_init_userspace:
    pushcpu
    