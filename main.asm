%include 'constants_values.inc'

section .text
    global _start

_start:

    ; defining the port value
    mov word[port], 0x1b39                      ; port(6969) == port(0x1b39)
    %include 'server_process.inc'

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
    mov rax, SYSCALL_READ
    mov rdi, [new_socket_fd]
    mov rsi, request_buf
    mov rdx, REQUEST_BUFFER_SIZE  
    syscall

    ; return value validation
    cmp rax, 0x00
    jge .read_http_request_success

.read_http_request_error:
    jmp .close_fd_befor_process_exit_error

.read_http_request_success:
    mov qword[request_len], rax                 ; mov the request length to buff

; --------------------------------------------------------------------------------------------------------
; Print the http request
; --------------------------------------------------------------------------------------------------------
; Description:  print the http request just for test  
;     
; - rax: syscall NR
; - rdi: the file descriptor standard output
; - rsi: the pointer of buffer with http request message
; - rdx: count bytes of buffer; the size is returned syscall read
;
; Return value: On success, the number of bytes read is returned.
;               On error, -1 is returned.
; --------------------------------------------------------------------------------------------------------

    ; syscall write
    mov rax, SYSCALL_WRITE
    mov rdi, STANDARD_OUTPUT_FD
    mov rsi, request_buf
    mov rdx, [request_len]   
    syscall

; --------------------------------------------------------------------------------------------------------
; Close the file descriptor before process exit
; --------------------------------------------------------------------------------------------------------
; Description:  it closes a file descriptor, so that it no longer refers to any file and may be reused.
;     
; - rax: syscall NR
; - rdi: the server file descriptor (server_fd)
;
; Return value: On success, these system calls return a file descriptor for the accepted socket.
;               On error, -1 is returned.
; --------------------------------------------------------------------------------------------------------

.close_fd_befor_process_exit_success:
    ; close the client file descriptor
    mov rax, SYSCALL_CLOSE
    mov rdi, [new_socket_fd]
    syscall

    ; close the server file descriptor
    mov rax, SYSCALL_CLOSE
    mov rdi, [server_fd]
    syscall

    jmp .process_exit_success

.close_fd_befor_process_exit_error:
    ; close the server file descriptor
    mov rax, SYSCALL_CLOSE
    mov rdi, [server_fd]
    syscall


    %include 'process_exit.inc'

section .data


; -----------------------------------------------------------------------------
;   File descriptor
; -----------------------------------------------------------------------------

    server_fd                   dq      -1 
    new_socket_fd               dq      -1

; -----------------------------------------------------------------------------
;   Structs sockaddr_in - Server and Client
; -----------------------------------------------------------------------------

    ; struct servaddr (sockaddr_in type)
    server_addr.sin_family      dw      0x00
    server_addr.sin_port        dw      0x00
    server_addr.sin_addr        dd      0x00
    server_addr.sin_padding     dq      0x00 

    ; struct cliaddr (sockaddr_in type)
    client_addr.sin_family      dw      0x00
    client_addr.sin_port        dw      0x00
    client_addr.sin_addr        dd      0x00
    client_addr.sin_padding     dq      0x00

    ; size of struct cliaddr
    client_addr_len             dq      0x10



section .bss

; -----------------------------------------------------------------------------
;   Request
; -----------------------------------------------------------------------------

    request_buf                 resb    0x1F40      ; maximum 8,000 bytes on URI (source: RFC 9110 section 4.1)
    request_len                 resq    0x01

; -----------------------------------------------------------------------------
;   PORT
; -----------------------------------------------------------------------------

    port                        resw    0x01
    port_htons                  resw    0x01

