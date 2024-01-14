section .data

    client_addr.sin_family      dw      0x00
    client_addr.sin_port        dw      0x00
    client_addr.sin_addr        dd      0x00
    client_addr.sin_padding     dq      0x00

    client_addr_len             equ     0x10
    client_addr_len_pointer     dq      client_addr_len

    socket_connection_fd        dq      -1                           

    request_max_size            equ     0x1F40              ; 8,000 octets in protocol elements (RFC 9110 section 4.1)

section .bss

    request_buf                 resb    request_max_size 
    request_buf_len             resq    0x01

section .text 

_pull_connection:
    mov rax, SYSCALL_ACCEPT
    mov rdi, [socket_server_fd]         ; created on 'network_socket/main.inc'
    mov rsi, client_addr.sin_family
    mov rdx, client_addr_len_pointer
    syscall 

    cmp rax, 0x00
    jl .pull_connection_end
    mov qword[socket_connection_fd], rax

    mov rax, SYSCALL_READ
    mov rdi, [socket_connection_fd]
    mov rsi, request_buf
    mov rdx, request_max_size
    syscall

    cmp rax, 0x00
    jl .pull_connection_end
    mov qword[request_buf_len], rax

    .pull_connection_end:
ret