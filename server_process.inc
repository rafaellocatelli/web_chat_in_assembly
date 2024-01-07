section .text

    ; print the start message 
    mov rax, SYSCALL_WRITE                      ; syscall:  write
    mov rdi, STANDARD_OUTPUT_FD                 ; arg0:     the file descriptor of standard output
    mov rsi, info_start                         ; arg1:     buffer pointer
    mov rdx, info_start_len                     ; arg2:     size of buffer in bytes
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
    mov rax, SYSCALL_SOCKET
    mov rdi, AF_INET
    mov rsi, SOCK_STREAM
    mov rdx, IPPROTO_IP
    syscall

    ; return value validation
    cmp rax, 0x00
    jge .socket_success

.socket_error:
    ; print the "INFO: socket"
    mov rax, SYSCALL_WRITE                      ; syscall:  write
    mov rdi, STANDARD_OUTPUT_FD                 ; arg0:     the file descriptor of standard output
    mov rsi, info_socket                        ; arg1:     buffer pointer
    mov rdx, info_socket_len                    ; arg2:     size of buffer in bytes
    syscall

    ; print "FAIL" in line of "INFO: socket"
    mov rax, SYSCALL_WRITE                      ; syscall:  write
    mov rdi, STANDARD_OUTPUT_FD                 ; arg0:     Standard Output
    mov rsi, info_fail                          ; arg1:     *buf
    mov rdx, info_fail_len                      ; arg2:     size of buf
    syscall

    jmp .process_exit_error

.socket_success:
    mov qword[server_fd], rax                   ; mov the file descriptor to buffer

    ; print the "INFO: socket"
    mov rax, SYSCALL_WRITE                      ; syscall:  write
    mov rdi, STANDARD_OUTPUT_FD                 ; arg0:     the file descriptor of standard output
    mov rsi, info_socket                        ; arg1:     buffer pointer
    mov rdx, info_socket_len                    ; arg2:     size of buffer in bytes
    syscall
    
    ; print "OK" in line of "INFO: socket"
    mov rax, SYSCALL_WRITE                      ; syscall:  write
    mov rdi, STANDARD_OUTPUT_FD                 ; arg0:     Standard Output
    mov rsi, info_success                       ; arg1:     *buf
    mov rdx, info_success_len                   ; arg2:     size of buf
    syscall


; --------------------------------------------------------------------------------------------------------
; Converting to port_htons
; --------------------------------------------------------------------------------------------------------
; Description:  sin_port contains the port in network byte order.
;               here we converts the unsigned integer hostlong from host byte order to network byte order.
; --------------------------------------------------------------------------------------------------------

    ; swap bytes
    ; the final result will be port_htons(0x391b)
    mov ax, [port]                              ; ax value is 0x1b39
    rol ax, 0x08                                ; change byte, ax value is 0x391b 
    mov word[port_htons], ax                    ; move ax value to port_htons buffer     

; --------------------------------------------------------------------------------------------------------
; Defining the server socket address structure 
; --------------------------------------------------------------------------------------------------------
; Description:  here we define the server socket address structure.
;               you can find more about the struture in linux man-page ip(7).
;
;   socket address structure:
;
;   name        - size              - description
;   ------------+-------------------+-------------------
;   sin_family  - 2 bytes (16-bits) - address family
;   sin_port    - 2 bytes (16-bits) - port in network byte order
;   sin_addr    - 4 bytes (32-bits) - address in network byte order
;   sin_padding - 8 bytes (64-bits) - padding
;
; --------------------------------------------------------------------------------------------------------

    ; server_addr.sin_family  =   AF_INET
    mov word[server_addr.sin_family], AF_INET

    ; server_addr.sin_port    =   htons(6969)
    mov ax, [port_htons]
    mov word[server_addr.sin_port], ax

    ; server_addr.sin_addr will be defined by syscall bind

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
    mov rax, SYSCALL_BIND
    mov rdi, [server_fd]
    mov rsi, server_addr.sin_family
    mov rdx, SOCKET_ADDRESS_STRUCT_SIZE
    syscall

    ; return value validation
    cmp rax, 0x00
    je .bind_success

.bind_error:
    ; print the "INFO: bind"
    mov rax, SYSCALL_WRITE                      ; syscall:  write
    mov rdi, STANDARD_OUTPUT_FD                 ; arg0:     Standard Output
    mov rsi, info_bind                          ; arg1:     *buf
    mov rdx, info_bind_len                      ; arg2:     size of buf
    syscall

    ; print "FAIL" in line of "INFO: bind"
    mov rax, SYSCALL_WRITE                      ; syscall:  write
    mov rdi, STANDARD_OUTPUT_FD                 ; arg0:     Standard Output
    mov rsi, info_fail                          ; arg1:     *buf
    mov rdx, info_fail_len                      ; arg2:     size of buf
    syscall

    jmp .close_fd_befor_process_exit_error

.bind_success:
    ; print the "INFO: bind"
    mov rax, SYSCALL_WRITE                      ; syscall:  write
    mov rdi, STANDARD_OUTPUT_FD                 ; arg0:     Standard Output
    mov rsi, info_bind                          ; arg1:     *buf
    mov rdx, info_bind_len                      ; arg2:     size of buf
    syscall
    
    ; print "OK" in line of "INFO: bind"
    mov rax, SYSCALL_WRITE                      ; syscall:  write
    mov rdi, STANDARD_OUTPUT_FD                 ; arg0:     Standard Output
    mov rsi, info_success                       ; arg1:     *buf
    mov rdx, info_success_len                   ; arg2:     size of buf
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
    mov rax, SYSCALL_LISTEN
    mov rdi, [server_fd]
    mov rsi, MAXIMUM_CONNECTIONS
    syscall

    ; return value validation
    cmp rax, 0x00
    je .listen_success

.listen_error:
    ; print the "INFO: listen"
    mov rax, SYSCALL_WRITE                      ; syscall:  write
    mov rdi, STANDARD_OUTPUT_FD                 ; arg0:     Standard Output
    mov rsi, info_listen                        ; arg1:     *buf
    mov rdx, 0x17                               ; arg2:     size of buf
    syscall

    ; print "FAIL" in line of "INFO: listen"
    mov rax, SYSCALL_WRITE                      ; syscall:  write
    mov rdi, STANDARD_OUTPUT_FD                 ; arg0:     Standard Output
    mov rsi, info_fail                          ; arg1:     *buf
    mov rdx, 0x15                               ; arg2:     size of buf
    syscall

    jmp .close_fd_befor_process_exit_error

.listen_success:
    ; print the "INFO: listen"
    mov rax, SYSCALL_WRITE                      ; syscall:  write
    mov rdi, STANDARD_OUTPUT_FD                 ; arg0:     Standard Output
    mov rsi, info_listen                        ; arg1:     *buf
    mov rdx, 0x17                               ; arg2:     size of buf
    syscall

    ; print "OK" in line of "INFO: listen"
    mov rax, SYSCALL_WRITE                      ; syscall:  write
    mov rdi, STANDARD_OUTPUT_FD                 ; arg0:     Standard Output
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
    mov rax, SYSCALL_WRITE                      ; syscall:  write
    mov rdi, STANDARD_OUTPUT_FD                 ; arg0:     Standard Output
    mov rsi, info_accept_success                ; arg1:     *buf
    mov rdx, 0x054                              ; arg2:     size of buf
    syscall

    ; syscall accept
    mov rax, SYSCALL_ACCEPT
    mov rdi, [server_fd]
    mov rsi, client_addr.sin_family
    mov rdx, client_addr_len
    syscall

    ; return value validation
    cmp rax, 0x00
    jge .accept_success

.accept_error:
    ; print "INFO: Accept FAIL"
    mov rax, SYSCALL_WRITE                      ; syscall:  write
    mov rdi, STANDARD_OUTPUT_FD                 ; arg0:     Standard Output
    mov rsi, info_accept_error                  ; arg1:     *buf
    mov rdx, 0x054                              ; arg2:     size of buf
    syscall

    jmp .close_fd_befor_process_exit_error

.accept_success:
    mov qword[new_socket_fd], rax               ; mov the file descriptor to buffer