; lock.asm
; Copyright Shreyas Lad (PenetratingShot) 2020
;
; Locks

global spinlock_lock
global spinlock_unlock

spinlock_lock:
    lock bts QWORD [rdi], 0
    jc spin
    ret

spin:
    pause
    test QWORD [rdi], 1
    jnz spin
    jmp spinlock_lock

spinlock_unlock:
    lock btr QWORD [rdi], 0
    ret