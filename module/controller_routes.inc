section .text

    mov rax, SYSCALL_READ
    mov rdi, [new_socket_fd]
    mov rsi, request_buf
    mov rdx, REQUEST_BUFFER_SIZE  
    syscall

    cmp rax, 0x00
    jge .read_http_request_success

.read_http_request_error:
    jmp .close_fd_befor_process_exit_error

.read_http_request_success:
    mov qword[request_len], rax

    mov rax, SYSCALL_WRITE
    mov rdi, STANDARD_OUTPUT_FD
    mov rsi, request_buf
    mov rdx, [request_len]   
    syscall
