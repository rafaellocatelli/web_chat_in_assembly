section .data

; --------------------------------------------------------------------------------------------------------
; Socket's constants values
; --------------------------------------------------------------------------------------------------------
; Description:  all are defined in the header file sys/socket.h         
; --------------------------------------------------------------------------------------------------------

    ; use on syscall socket
    AF_INET                     equ     0x02        ; /* internetwork: UDP, TCP, etc. */
    SOCK_STREAM                 equ     0x01        ; /* stream socket */
    IPPROTO_IP                  equ     0x00        ; Dummy protocol for TCP
    
    ; use on syscall setsockopt
    SOL_SOCKET                  equ     0xffff      ; /* options for socket level */
    SO_REUSEADDR                equ     0x04        ; /* allow local address reuse */



; --------------------------------------------------------------------------------------------------------
; Linux x86_64 (64-bit) System Call Table
; --------------------------------------------------------------------------------------------------------
; Description:  Here's a sheet for the syscall calling  i used in this app
; 
; See also:
;               https://man7.org/linux/man-pages/man2/syscall.2.html
;               https://chromium.googlesource.com/chromiumos/docs/+/master/constants/syscalls.md       
; --------------------------------------------------------------------------------------------------------

    SYSCALL_READ                equ     0x00
    SYSCALL_WRITE               equ     0x01
    SYSCALL_CLOSE               equ     0x03
    SYSCALL_SOCKET              equ     0x29
    SYSCALL_ACCEPT              equ     0x2b
    SYSCALL_BIND                equ     0x31        ; int bind(int sockfd, const struct sockaddr *addr, socklen_t addrlen);
    SYSCALL_LISTEN              equ     0x32        ; int listen(int sockfd, int backlog);
    SYSCALL_SETSOCKOPT          equ     0x36        ; int setsockopt(int sockfd, int level, int optname, const void *optval, socklen_t optlen);
    SYSCALL_EXIT                equ     0x3c        ; void _exit(int status);



    STANDARD_INPUT_FD           equ     0x00
    STANDARD_OUTPUT_FD          equ     0x01


    SOCKET_ADDRESS_STRUCT_SIZE  equ     0x10


    MAXIMUM_CONNECTIONS         equ     0x05

    REQUEST_BUFFER_SIZE         equ     0x1F40      ; size of buffer - maximum 8,000 bytes on URI (source: RFC 9110 section 4.1)

    PROCESS_EXIT_NORMALLY       equ     0x00
    PROCESS_EXIT_ERROR          equ     0x00
