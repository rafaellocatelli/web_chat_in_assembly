section .data

    server_fd                   dq      -1 
    new_socket_fd               dq      -1

    server_addr.sin_family      dw      0x00
    server_addr.sin_port        dw      0x00
    server_addr.sin_addr        dd      0x00
    server_addr.sin_padding     dq      0x00 

    client_addr.sin_family      dw      0x00
    client_addr.sin_port        dw      0x00
    client_addr.sin_addr        dd      0x00
    client_addr.sin_padding     dq      0x00

    client_addr_len             dq      0x10

section .bss

    request_buf                 resb    REQUEST_BUFFER_SIZE
    request_len                 resq    0x01

    port                        resw    0x01
    port_htons                  resw    0x01