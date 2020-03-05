; syscall.asm
; Copyright Shreyas Lad (PenetratingShot) 2020
;
; Syscall stub handler

[bits 64]

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

; rax: holds the syscall action
; rdi: holds the first argument
; rsi: holds the second argument
; rdx: holds the third argument
; rax: holds the return value

[extern syscallHandler]
[extern kernel_stack_top]

global _entersys
global _extsys
global _chgrip

_entersys:
    mov rsp, [kernel_stack_top]
    pushaq
    mov rbx, rsp
    call syscallHandler

    jmp _extsys

_extsys:
    mov rsp, rbx
    popaq
    
    sysret

_chgrip:
    mov rcx, rdi
    sysretq