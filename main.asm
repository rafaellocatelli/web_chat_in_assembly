section .data

    hello_world             db      'Olá mundo!', 0x0a
    hello_world_len         equ     $ - hello_world

section .text
    global _start

_start:
    call    _print
    call    _process_exit

_process_exit:
    mov     rax, 0x3c               ; syscall:  exit
    mov     rdi, 0x00               ; arg0:     process exit normally
    syscall

_print:
    mov     rax, 0x01               ; syscall:  write
    mov     rdi, 0x01               ; arg0:     Standard output
    mov     rsi, hello_world        ; arg1:     String pointer
    mov     rdx, hello_world_len    ; arg2:     Size of String
    syscall
    ret