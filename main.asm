section .text
    global _start

_start:

    ; print the start message 
    mov rax, 0x01                               ; syscall:  write
    mov rdi, 0x01                               ; arg0:     the file descriptor of standard output
    mov rsi, info_start                         ; arg1:     buffer pointer
    mov rdx, 0x17                               ; arg2:     size of buffer in bytes
    syscall

; --------------------------------------------------------------------------------------------------------
; Server process - Socket
; --------------------------------------------------------------------------------------------------------
; Description:  syscall socket creates  an  endpoint  for communication 
;               and returns a file descriptor that refers to that endpoint.
;     
; - rax: syscall NR
; - rdi: specifies communication domain, AF_INET (IPv4 Internet protocols) as defined
; - rsi: specifies communication type, SOCK_STREAM (Transmission Control Protocol - TCP) as defined
; - rdx: specifies a particular protocol, IPPROTO_IP (Dummy protocol for TCP) as defined
;
; Return value: On success, a file descriptor for the new socket is returned.
;               On error, -1 is returned.
; --------------------------------------------------------------------------------------------------------

    ; syscall socket
    mov rax, 0x29
    mov rdi, 0x02
    mov rsi, 0x01
    mov rdx, 0x00
    syscall

    ; return value validation
    cmp rax, 0x00
    jge  .socket_success

.socket_error:
    ; print the "INFO: socket"
    mov rax, 0x01                               ; syscall:  write
    mov rdi, 0x01                               ; arg0:     the file descriptor of standard output
    mov rsi, info_socket                        ; arg1:     buffer pointer
    mov rdx, 0x17                               ; arg2:     size of buffer in bytes
    syscall

    ; print "FAIL" in line of "INFO: socket"
    mov rax, 0x01                               ; syscall:  write
    mov rdi, 0x01                               ; arg0:     Standard Output
    mov rsi, info_fail                          ; arg1:     *buf
    mov rdx, 0x15                               ; arg2:     size of buf
    syscall

    jmp .process_exit_error

.socket_success:
    mov qword[server_fd], rax                   ; mov the file descriptor to buffer

    ; print the "INFO: socket"
    mov rax, 0x01                               ; syscall:  write
    mov rdi, 0x01                               ; arg0:     the file descriptor of standard output
    mov rsi, info_socket                        ; arg1:     buffer pointer
    mov rdx, 0x17                               ; arg2:     size of buffer in bytes
    syscall
    
    ; print "OK" in line of "INFO: socket"
    mov rax, 0x01                               ; syscall:  write
    mov rdi, 0x01                               ; arg0:     Standard Output
    mov rsi, info_success                       ; arg1:     *buf
    mov rdx, 0x15                               ; arg2:     size of buf
    syscall

; --------------------------------------------------------------------------------------------------------
; Server process - Bind
; --------------------------------------------------------------------------------------------------------
; Description:  syscall bind assigns the address specified by "rsi" 
;               to the socket referred by the file descriptor on "rdi".
;     
; - rax: syscall NR
; - rdi: the file descriptor returned by syscall socket
; - rsi: the address structure pointed
; - rdx: specifies the size of structure in bytes
;
; Return value: On success, zero is returned.
;               On error, -1 is returned.
; --------------------------------------------------------------------------------------------------------

    ; syscall bind
    mov rax, 0x31
    mov rdi, [server_fd]
    mov rsi, servaddr.sin_family
    mov rdx, 0x10
    syscall

    ; return value validation
    cmp rax, 0x00
    je .bind_success

.bind_error:
    ; print the "INFO: bind"
    mov rax, 0x01                               ; syscall:  write
    mov rdi, 0x01                               ; arg0:     Standard Output
    mov rsi, info_bind                          ; arg1:     *buf
    mov rdx, 0x17                               ; arg2:     size of buf
    syscall

    ; print "FAIL" in line of "INFO: bind"
    mov rax, 0x01                               ; syscall:  write
    mov rdi, 0x01                               ; arg0:     Standard Output
    mov rsi, info_fail                          ; arg1:     *buf
    mov rdx, 0x15                               ; arg2:     size of buf
    syscall

    jmp .close_fd_befor_process_exit_error

.bind_success:
    ; print the "INFO: bind"
    mov rax, 0x01                               ; syscall:  write
    mov rdi, 0x01                               ; arg0:     Standard Output
    mov rsi, info_bind                          ; arg1:     *buf
    mov rdx, 0x17                               ; arg2:     size of buf
    syscall
    
    ; print "OK" in line of "INFO: bind"
    mov rax, 0x01                               ; syscall:  write
    mov rdi, 0x01                               ; arg0:     Standard Output
    mov rsi, info_success                       ; arg1:     *buf
    mov rdx, 0x15                               ; arg2:     size of buf
    syscall

; --------------------------------------------------------------------------------------------------------
; Server process - Listen
; --------------------------------------------------------------------------------------------------------
; Description:  syscall listen marks the socket referred to by sockfd as a passive socket, that is, 
;               as a socket that will be used to accept incoming connection requests using syscall accept.
;     
; - rax: syscall NR
; - rdi: the file descriptor returned by syscall socket
; - rsi: defines the maximum length to which the queue of pending connections for "server_fd" may grow
;
; Return value: On success, zero is returned.
;               On error, -1 is returned.
; --------------------------------------------------------------------------------------------------------

    ; syscall listen
    mov rax, 0x32
    mov rdi, [server_fd]
    mov rsi, 0x05
    syscall

    ; return value validation
    cmp rax, 0x00
    je .listen_success

.listen_error:
    ; print the "INFO: listen"
    mov rax, 0x01                               ; syscall:  write
    mov rdi, 0x01                               ; arg0:     Standard Output
    mov rsi, info_listen                        ; arg1:     *buf
    mov rdx, 0x17                               ; arg2:     size of buf
    syscall

    ; print "FAIL" in line of "INFO: listen"
    mov rax, 0x01                               ; syscall:  write
    mov rdi, 0x01                               ; arg0:     Standard Output
    mov rsi, info_fail                          ; arg1:     *buf
    mov rdx, 0x15                               ; arg2:     size of buf
    syscall

    jmp .close_fd_befor_process_exit_error

.listen_success:
    ; print the "INFO: listen"
    mov rax, 0x01                               ; syscall:  write
    mov rdi, 0x01                               ; arg0:     Standard Output
    mov rsi, info_listen                        ; arg1:     *buf
    mov rdx, 0x17                               ; arg2:     size of buf
    syscall

    ; print "OK" in line of "INFO: listen"
    mov rax, 0x01                               ; syscall:  write
    mov rdi, 0x01                               ; arg0:     Standard Output
    mov rsi, info_success                       ; arg1:     *buf
    mov rdx, 0x15                               ; arg2:     size of buf
    syscall

; --------------------------------------------------------------------------------------------------------
; Server process - Accept
; --------------------------------------------------------------------------------------------------------
; Description:  syscall accept extracts the first connection request on the queue of pending connections 
;               for the listening socket (server_fd), creates a new connected socket 
;               and returns a new file descriptor (new_socket_fd) referring to that socket. 
;     
; - rax: syscall NR
; - rdi: the file descriptor that has been created with syscall socket
; - rsi: the address struct cliaddr pointed
; - rdx: the pointer to address that contain the size in bytes of struct cliaddr
;
; Return value: On success, these system calls return a file descriptor for the accepted socket.
;               On error, -1 is returned.
; --------------------------------------------------------------------------------------------------------

    ; print a complete message of new state server
    mov rax, 0x01                               ; syscall:  write
    mov rdi, 0x01                               ; arg0:     Standard Output
    mov rsi, info_accept_success                ; arg1:     *buf
    mov rdx, 0x054                              ; arg2:     size of buf
    syscall

    ; syscall accept
    mov rax, 0x2b
    mov rdi, [server_fd]
    mov rsi, cliaddr.sin_family
    mov rdx, cliaddr_len
    syscall

    ; return value validation
    cmp rax, 0x00
    jge .accept_success

.accept_error:
    ; print "INFO: Accept FAIL"
    mov rax, 0x01                               ; syscall:  write
    mov rdi, 0x01                               ; arg0:     Standard Output
    mov rsi, info_accept_error                  ; arg1:     *buf
    mov rdx, 0x054                              ; arg2:     size of buf
    syscall

    jmp .close_fd_befor_process_exit_error

.accept_success:
    mov qword[new_socket_fd], rax               ; mov the file descriptor to buffer

; --------------------------------------------------------------------------------------------------------
; Read the http request
; --------------------------------------------------------------------------------------------------------
; Description:  syscall accept extracts the first connection request on the queue of pending connections 
;               for the listening socket (server_fd), creates a new connected socket 
;               and returns a new file descriptor (new_socket_fd) referring to that socket. 
;     
; - rax: syscall NR
; - rdi: the cliente file descriptor (new_socket_fd)
; - rsi: the pointer of buffer
; - rdx: count bytes of buffer; size of buffer - maximum 8,000 bytes on URI (source: RFC 9110 section 4.1)
;
; Return value: On success, the number of bytes read is returned.
;               On error, -1 is returned.
; --------------------------------------------------------------------------------------------------------

    ; syscall read
    mov rax, 0x00
    mov rdi, [new_socket_fd]
    mov rsi, request_buf
    mov rdx, 0x1F40  
    syscall

    ; return value validation
    cmp rax, 0x00
    jge .read_http_request_success

.read_http_request_error:
    jmp .close_fd_befor_process_exit_error

.read_http_request_success:
    mov qword[request_len], rax             ; mov the request length to buff

    ; print all request message
    mov rax, 0x01                           ; syscall:  write
    mov rdi, 0x01                           ; arg0:     Standard Output 
    mov rsi, request_buf                    ; arg1:     char *buf
    mov rdx, [request_len]                  ; arg2:     buf length     
    syscall

    ; close the client file descriptor
    mov rax, 0x03                               ; syscall:  close
    mov rdi, [new_socket_fd]                    ; arg0:     the file descriptor sockfd
    syscall

    ; close the server file descriptor
    mov rax, 0x03                               ; syscall:  close
    mov rdi, [server_fd]                        ; arg0:     the file descriptor sockfd
    syscall

    jmp .process_exit_success

; --------------------------------------------------------------------------------------------------------
; Close the server file descriptor before process exit error
; --------------------------------------------------------------------------------------------------------
; Description:  it closes a file descriptor, so that it no longer refers to any file and may be reused.
;     
; - rax: syscall NR
; - rdi: the server file descriptor (server_fd)
;
; Return value: On success, these system calls return a file descriptor for the accepted socket.
;               On error, -1 is returned.
; --------------------------------------------------------------------------------------------------------

.close_fd_befor_process_exit_error:
    ; syscall close
    mov rax, 0x03
    mov rdi, [server_fd]
    syscall

; --------------------------------------------------------------------------------------------------------
; Process exit
; --------------------------------------------------------------------------------------------------------
; Description:  syscall exit terminates the calling process "immediately".
;               
; - rax: syscall NR
; - rdi: the value status
;           0 == exit normally
;           0 != exit fail
;
; Return value: These functions do not return.
; --------------------------------------------------------------------------------------------------------

.process_exit_error:
    ; syscall exit 
    mov rax, 0x3c
    mov rdi, 0x01
    syscall

.process_exit_success:
    ; print information about closing web server 
    mov rax, 0x01                           ; syscall:  write
    mov rdi, 0x01                           ; arg0:     Standard Output 
    mov rsi, info_close                     ; arg1:     char *buf
    mov rdx, 0x30                           ; arg2:     buf length     
    syscall

    ; syscall exit
    mov rax, 0x3c
    mov rdi, 0x00
    syscall

section .data

; -----------------------------------------------------------------------------
;   Information messages
; -----------------------------------------------------------------------------

    info_success            db      `\033[01;32mSUCCESS\033[00m`, 0x0a
    info_fail               db      `\033[01;31mFAIL   \033[00m`, 0x0a

    info_start              db      'Starting web server...', 0x0a
    info_socket             db      `\033[01mINFO:\033[00m socket `
    info_bind               db      `\033[01mINFO:\033[00m bind   `
    info_listen             db      `\033[01mINFO:\033[00m listen `
    info_accept_error       db      `\033[01mINFO:\033[00m Accept `
                            db      `\033[01;31mFAIL   \033[00m`, 0x0a
    info_accept_success     db      0x0a, `Web Server \033[32mon\033[00m`, 0x0a
                            db      'connect on http://127.0.0.1:6969 or http://localhost:6969', 0x0a, 0x0a
    info_close              db      0x0a, 'Closing web server...', 0x0a
                            db      `Web Server \033[31moff\033[00m`, 0x0a

; -----------------------------------------------------------------------------
;   Fil descriptor
; -----------------------------------------------------------------------------

    server_fd               dq      -1 
    new_socket_fd           dq      -1

; -----------------------------------------------------------------------------
;   Structs sockaddr_in - Server and Client
; -----------------------------------------------------------------------------

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

; -----------------------------------------------------------------------------
;   Request
; -----------------------------------------------------------------------------

    request_buf             resb    0x1F40      ; maximum 8,000 bytes on URI (source: RFC 9110 section 4.1)
    request_len             resq    0x01
    request_cur             resq    0x01
