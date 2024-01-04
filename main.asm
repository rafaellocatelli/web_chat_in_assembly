section .data

    ; file descriptor
    sockfd  dq  -1 
    connfd  dq  -1

    ; struct servaddr (sockaddr_in type)
    servaddr.sin_family     dw      0x02        ; sa_family=AF_INET
    servaddr.sin_port       dw      0x391B      ; sin_port=htons(6969)
    servaddr.sin_addr       dd      0x00
    servaddr.sin_padding    dq      0x00 

    ; struct cliaddr (sockaddr_in type)
    cliaddr.sin_family      dw      0x00
    cliaddr.sin_port        dw      0x00
    cliaddr.sin_addr        dd      0x00
    cliaddr.sin_padding     dq      0x00

    ; size of struct cliaddr
    cliaddr_len             dq      0x10

section .bss

    request_buf             resb    0x1F40      ; maximum 8,000 bytes on URI (source: RFC 9110 section 4.1)
    request_len             resq    0x01
    request_cur             resq    0x01

section .text
    global _start

_start:
    mov     rax, 0x29                           ; syscall:  socket
    mov     rdi, 0x02                           ; arg0:     AF_INET
    mov     rsi, 0x01                           ; arg1:     SOCK_STREAM
    mov     rdx, 0x00                           ; arg2:     IPPROTO_IP
    syscall

    ; socket success?
    cmp     rax, 0x00
    jl      .exit_error
    mov     qword[sockfd], rax

    mov     rax, 0x31                           ; syscall:  bind
    mov     rdi, [sockfd]                       ; arg0:     the file descriptor sockfd
    mov     rsi, servaddr.sin_family            ; arg1:     struct servaddr pointer
    mov     rdx, 0x10                           ; arg2:     servaddr length
    syscall

    ; bind success?
    cmp     rax, 0x00
    jne     .exit_error

    mov     rax, 0x32                           ; syscall:  listen
    mov     rdi, [sockfd]                       ; arg0:     the file descriptor sockfd
    mov     rsi, 0x05                           ; arg1:     maximum 5 connections
    syscall

    ; listen success?
    cmp     rax, 0x00
    jne     .exit_error

    mov     rax, 0x2b                           ; syscall:  accept
    mov     rdi, [sockfd]                       ; arg0:     the file descriptor sockfd
    mov     rsi, cliaddr.sin_family             ; arg1:     struct cliaddr pointer
    mov     rdx, cliaddr_len                    ; arg2:     cliaddr length pointer
    syscall

    ; accept success?
    cmp     rax, 0x00
    jl      .exit_error
    mov     qword[connfd], rax

    mov     rax, 0x00                           ; syscall:  read
    mov     rdi, [connfd]                       ; arg0:     the file descriptor of connection
    mov     rsi, request_buf                    ; arg1:     *buf
    mov     rdx, 0x1F40                         ; arg2:     size of buffer - maximum 8,000 bytes on URI (source: RFC 9110 section 4.1)
    syscall

    ; read success?
    cmp     rax, 0x00
    jl      .exit_error
    mov     qword[request_len], rax

    mov     rax, 0x01                           ; syscall:  write
    mov     rdi, 0x01                           ; arg0:     Standard Output 
    mov     rsi, request_buf                    ; arg1:     char *buf
    mov     rdx, [request_len]                  ; arg2:     buf length     
    syscall

    mov     rax, 0x03                           ; syscall:  close
    mov     rdi, [connfd]                       ; arg0:     the file descriptor sockfd
    syscall

    mov     rax, 0x03                           ; syscall:  close
    mov     rdi, [sockfd]                       ; arg0:     the file descriptor sockfd
    syscall

.exit_success:
    mov     rax, 0x3c                           ; syscall:  exit
    mov     rdi, 0x00                           ; arg0:     process exit normally
    syscall

.exit_error:
    mov     rax, 0x03                           ; syscall:  close
    mov     rdi, [sockfd]                       ; arg0:     the file descriptor sockfd
    syscall
    
    mov     rax, 0x3c                           ; syscall:  exit
    mov     rdi, 0x01                           ; arg0:     process exit normally
    syscall