; syscall.asm
; Copyright Shreyas Lad (PenetratingShot) 2020
;
; Syscall stub handler

[bits 64]
[extern vfsread]
[extern vfswrite]
[extern getNodeFromEntry]
[extern kernel_stack_top]

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

; I absolutely hate all of this code
syscall_table:
    dq _sysread
    dq _syswrite

global _init_syscall
; rax: holds the syscall action
; rbx: holds the user stack
; r8: holds the first argument
; r9: holds the second argument
; r10: holds the third argument

_init_syscall:
    ; load the address into rcx later on
    mov rax, _syscall_stub
    mov rdx, _syscall_stub
    shr rdx, 32 ; change to shl if shit doesn't work
    wrmsr

_syscall_stub:
    pushaq
    mov rbx, rsp
    mov rsp, kernel_stack_top ; assume the user stack has been setup
    call [syscall_table + rax * 8]

    popaq
    mov rsp, rbx ; setup the userspace stack again
    sysret

_sysread:


_syswrite: