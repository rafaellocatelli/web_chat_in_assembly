section .data

; --------------------------------------------------------------------------------------------------------
; File descriptor values
; --------------------------------------------------------------------------------------------------------
; Descrição:
;               Essa são os valores usado na criação e configuração do socket do server.
;
;               É possível encontrar estes valores no diretório usr/include/[...] mas o caminho pode variar
;               para cada versão e imagem Linux.
;
;               Então eu sugiro identificar os valores pelos links:
;               https://github.com/openbsd/src/blob/master/sys/sys/socket.h
;  
; --------------------------------------------------------------------------------------------------------

    ; usamos no syscall socket
    AF_INET                     equ     0x02
    SOCK_STREAM                 equ     0x01
    IPPROTO_IP                  equ     0x00
  
    ; usamos no syscall setsockopt
    SOL_SOCKET                  equ     0xffff

    SO_REUSEADDR                equ     0x04

    SOCKET_ADDRESS_STRUCT_SIZE  equ     0x10

    MAXIMUM_CONNECTIONS         equ     0x05

    REQUEST_BUFFER_SIZE         equ     0x1F40