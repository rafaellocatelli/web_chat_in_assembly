section .data

    EXIT_STATUS_SUCESS  equ     0x00

section .text

_process_exit:
    mov rax, SYSCALL_EXIT
    mov rdi, EXIT_STATUS_SUCESS
    syscall