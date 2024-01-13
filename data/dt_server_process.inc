section .data

; --------------------------------------------------------------------------------------------------------
; Valores constantes utilizados no socket
; --------------------------------------------------------------------------------------------------------
; Descrição:
;               Estes são os valores usado na criação e configuração do socket do server.
;
;               É possível encontrar estes valores no diretório usr/include/[...] mas o caminho pode variar
;               para cada versão e imagem Linux.
;
;               Então eu sugiro identificar os valores pelos links:
;               https://github.com/openbsd/src/blob/master/sys/sys/socket.h
;               https://github.com/torvalds/linux/blob/master/include/uapi/asm-generic/socket.h
;  
; --------------------------------------------------------------------------------------------------------

    ; usamos no syscall socket
    AF_INET                     equ     0x02
    SOCK_STREAM                 equ     0x01
    IPPROTO_IP                  equ     0x00
  
    ; usamos no syscall setsockopt
    ;SOL_SOCKET                  equ     0xffff
    ;SO_REUSEADDR                equ     0x0004
    
; --------------------------------------------------------------------------------------------------------
; Variáveis exclusivas para socket address structure
; --------------------------------------------------------------------------------------------------------
; Descrição:
;               Não encontrei uma documentação. Mas é a somatória de todos os campos da socket address 
;               structure.
;
;
;               socket address structure ipv4:
;
;               name        - size              - description
;               ------------+-------------------+-------------------
;               sin_family  - 2 bytes (16-bits) - address family
;               sin_port    - 2 bytes (16-bits) - port in network byte order
;               sin_addr    - 4 bytes (32-bits) - address in network byte order
;               sin_padding - 8 bytes (64-bits) - padding
;
;               Total de 16 bytes
;
;               16 em hexadecimal é "10"
;  
; --------------------------------------------------------------------------------------------------------

    SOCKET_ADDRESS_STRUCT_SIZE  equ     0x10

    server_addr.sin_family      dw      0x00
    server_addr.sin_port        dw      0x00
    server_addr.sin_addr        dd      0x00
    server_addr.sin_padding     dq      0x00 

    client_addr.sin_family      dw      0x00
    client_addr.sin_port        dw      0x00
    client_addr.sin_addr        dd      0x00
    client_addr.sin_padding     dq      0x00

    client_addr_len             dq      SOCKET_ADDRESS_STRUCT_SIZE

; --------------------------------------------------------------------------------------------------------
; Máximo de conexões
; --------------------------------------------------------------------------------------------------------
; Descrição:
;               Este valor foi definido por mim de forma arbitrária
;  
; --------------------------------------------------------------------------------------------------------

    MAXIMUM_CONNECTIONS         equ     0x05

; --------------------------------------------------------------------------------------------------------
; Tamanho do buffer para HTTP Request
; --------------------------------------------------------------------------------------------------------
; Descrição:
;               Este valor foi obtido na seção 4.1 da documentação RFC 9110
;
;               Trecho de RFC 9110 section 4.1:
;                   "É RECOMENDADO que todos os remetentes e destinatários suportem, no mínimo, 
;                   URIs com comprimento de 8.000 octetos em elementos de protocolo."
;
;               1 octeto é o equivalente a 1 byte
;
;               8.000 em hexadecimal é "1F40"
;  
;               Link:
;               https://www.rfc-editor.org/rfc/rfc9110.html#section-4.1-1
;
; --------------------------------------------------------------------------------------------------------

    REQUEST_BUFFER_SIZE         equ     0x1F40

; --------------------------------------------------------------------------------------------------------
; Socket file descriptor
; --------------------------------------------------------------------------------------------------------
; Descrição:
;               Os socket file descriptors em questão são alimentados pelos syscalls socket e accept.
;               Ambos syscalls retornam "-1" em caso de insucesso.
;
;               Para termos a certeza de que os file descriptor são válidos, nós já inicializamos eles com
;               o valor "-1" (valor inválido) e se tornarão válidos se de fato os syscalls tiverem êxito.
;
; --------------------------------------------------------------------------------------------------------

    server_fd                   dq      -1 
    new_socket_fd               dq      -1

    open_fd                     dq      -1
    
    name_html                   db      'static/html/get_home.html', 0

section .bss

; --------------------------------------------------------------------------------------------------------
; Espaço reservado para as requests e responses
; --------------------------------------------------------------------------------------------------------

    request_buf                 resb    REQUEST_BUFFER_SIZE
    request_len                 resq    0x01

    response_buf                resb    0xffff
    response_len                resq    0x01


    open_html                   resq    0x01
    open_html_len               resq    0x01

; --------------------------------------------------------------------------------------------------------
; Espaço reservado para a definição da porta do socket
; --------------------------------------------------------------------------------------------------------

    port                        resw    0x01
    port_htons                  resw    0x01