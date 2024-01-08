section .text

.close_fd_befor_process_exit_success:
    mov rax, SYSCALL_CLOSE
    mov rdi, [new_socket_fd]
    syscall

    mov rax, SYSCALL_CLOSE
    mov rdi, [server_fd]
    syscall

    jmp .process_exit_success

.close_fd_befor_process_exit_error:
    mov rax, SYSCALL_CLOSE
    mov rdi, [server_fd]
    syscall

.process_exit_error:
    mov rax, SYSCALL_EXIT
    mov rdi, PROCESS_EXIT_ERROR
    syscall

.process_exit_success:
    mov rax, SYSCALL_WRITE
    mov rdi, STANDARD_OUTPUT_FD
    mov rsi, info_close
    mov rdx, info_close_len   
    syscall

    mov rax, SYSCALL_EXIT
    mov rdi, PROCESS_EXIT_NORMALLY
    syscall
