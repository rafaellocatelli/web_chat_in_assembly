section .data

    AF_INET                 equ 0x02        ; IPv4 Internet protocols
    SOCK_STREAM             equ 0x01        ; AF_INET + SOCK_STREAM = TPC
    IPPROTO_IP              equ 0x00        ; IPPROTO_IP é um protocolo fictício para TCP
    PORT                    equ 0x1B39      ; port 6969
    PORT_HTONS              equ 0x391B      ; port in network byte order
    INADDR_ANY              equ 0x00000000  ; INADDR_ANY (0.0.0.0) significa qualquer endereço para ligação
    MAX_CONNECTIONS         equ 0x05        ; máximo de conexões pendentes

    socket_server_fd        dq  -1

    server_addr.sin_family  dw  AF_INET
    server_addr.sin_port    dw  PORT_HTONS
    server_addr.sin_addr    dd  INADDR_ANY 
    server_addr.sin_padding dq  0x00

    server_addr_len         equ 0x10

section .text

_network_socket:

    mov rax, SYSCALL_SOCKET
    mov rdi, AF_INET
    mov rsi, SOCK_STREAM
    mov rdx, IPPROTO_IP
    syscall

    cmp rax, 0x00
    jl .network_socket_end
    mov qword[socket_server_fd], rax

    mov rax, SYSCALL_BIND
    mov rdi, [socket_server_fd]
    mov rsi, server_addr.sin_family
    mov rdx, server_addr_len
    syscall

    cmp rax, 0x00
    jne .network_socket_fail

    mov rax, SYSCALL_LISTEN
    mov rdi, [socket_server_fd]
    mov rsi, MAX_CONNECTIONS
    syscall

    .network_socket_end:
ret

    .network_socket_fail:
    mov rax, SYSCALL_CLOSE
    mov rdi, [socket_server_fd]
    syscall
ret