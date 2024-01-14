section .data

; NOT FOUND - -1
; GET       -  0
; POST      -  1
; PUT       -  2
; DELETE    -  3
; HEAD      -  4
; OPTIONS   -  5
; TRACE     -  6
; CONNECT   -  7

method  db  -1

section .text

_method_identify:
    ; request_buf_len   - tamanho do http request       source: 'pull_connection/main.inc'
    ; request_buf       - dados do http request         source: 'pull_connection/main.inc'

    .method_identify_first_char:
    mov al, [request_buf]

    cmp al, 'G'
    je .method_identify_get

    cmp al, 'P'
    je .method_identify_second_char

    cmp al, 'D'
    je .method_identify_delete

    cmp al, 'H'
    je .method_identify_head

    cmp al, 'O'
    je .method_identify_options

    cmp al, 'T'
    je .method_identify_trace

    cmp al, 'C'
    je .method_identify_connect

    jmp .method_identify_not_found

    .method_identify_second_char:
    ; already validated first char ('P')

    mov al, [request_buf + 0x01]

    cmp al, 'O'
    je .method_identify_post

    cmp al, 'U'
    je .method_identify_put

    jmp .method_identify_not_found

    ; --------------------------
    ;           GET
    ; --------------------------
    .method_identify_get:
    ; already validated first char ('G')

    mov al, [request_buf + 0x01]
    cmp al, 'E'
    jne .method_identify_not_found

    mov al, [request_buf + 0x02]
    cmp al, 'T'
    jne .method_identify_not_found

    mov al, [request_buf + 0x03]
    cmp al, ' '
    jne .method_identify_not_found

    mov byte[method], 0x00

ret

    ; --------------------------
    ;           POST
    ; --------------------------
    .method_identify_post:
    ; already validated first and second chars ('P' and 'O')

    mov al, [request_buf + 0x02]
    cmp al, 'S'
    jne .method_identify_not_found

    mov al, [request_buf + 0x03]
    cmp al, 'T'
    jne .method_identify_not_found

    mov al, [request_buf + 0x04]
    cmp al, ' '
    jne .method_identify_not_found

    mov byte[method], 0x01

ret

    ; --------------------------
    ;           PUT
    ; --------------------------
    .method_identify_put:
    ; already validated first and second chars ('P' and 'U')

    mov al, [request_buf + 0x02]
    cmp al, 'T'
    jne .method_identify_not_found

    mov al, [request_buf + 0x03]
    cmp al, ' '
    jne .method_identify_not_found

    mov byte[method], 0x02

ret

    ; --------------------------
    ;           DELETE
    ; --------------------------
    .method_identify_delete:
    ; already validated first char ('D')

    mov al, [request_buf + 0x01]
    cmp al, 'E'
    jne .method_identify_not_found

    mov al, [request_buf + 0x02]
    cmp al, 'L'
    jne .method_identify_not_found

    mov al, [request_buf + 0x03]
    cmp al, 'E'
    jne .method_identify_not_found

    mov al, [request_buf + 0x04]
    cmp al, 'T'
    jne .method_identify_not_found

    mov al, [request_buf + 0x05]
    cmp al, 'E'
    jne .method_identify_not_found

    mov al, [request_buf + 0x06]
    cmp al, ' '
    jne .method_identify_not_found

    mov byte[method], 0x03

ret

    ; --------------------------
    ;           HEAD
    ; --------------------------
    .method_identify_head:
    ; already validated first char ('H')

    mov al, [request_buf + 0x01]
    cmp al, 'E'
    jne .method_identify_not_found

    mov al, [request_buf + 0x02]
    cmp al, 'A'
    jne .method_identify_not_found

    mov al, [request_buf + 0x03]
    cmp al, 'D'
    jne .method_identify_not_found

    mov al, [request_buf + 0x03]
    cmp al, ' '
    jne .method_identify_not_found

    mov byte[method], 0x04

ret

    ; --------------------------
    ;           OPTIONS
    ; --------------------------
    .method_identify_options:
    ; already validated first char ('O')

    mov al, [request_buf + 0x01]
    cmp al, 'P'
    jne .method_identify_not_found

    mov al, [request_buf + 0x02]
    cmp al, 'T'
    jne .method_identify_not_found

    mov al, [request_buf + 0x03]
    cmp al, 'I'
    jne .method_identify_not_found

    mov al, [request_buf + 0x04]
    cmp al, 'O'
    jne .method_identify_not_found

    mov al, [request_buf + 0x05]
    cmp al, 'N'
    jne .method_identify_not_found

    mov al, [request_buf + 0x06]
    cmp al, 'S'
    jne .method_identify_not_found

    mov al, [request_buf + 0x07]
    cmp al, ' '
    jne .method_identify_not_found

    mov byte[method], 0x05

ret

    ; --------------------------
    ;           TRACE
    ; --------------------------
    .method_identify_trace:
    ; already validated first char ('T')

    mov al, [request_buf + 0x01]
    cmp al, 'R'
    jne .method_identify_not_found

    mov al, [request_buf + 0x02]
    cmp al, 'A'
    jne .method_identify_not_found

    mov al, [request_buf + 0x03]
    cmp al, 'C'
    jne .method_identify_not_found

    mov al, [request_buf + 0x04]
    cmp al, 'E'
    jne .method_identify_not_found

    mov al, [request_buf + 0x05]
    cmp al, ' '
    jne .method_identify_not_found

    mov byte[method], 0x06

ret

    ; --------------------------
    ;           CONNECT
    ; --------------------------
    .method_identify_connect:
    ; already validated first char ('C')

    mov al, [request_buf + 0x01]
    cmp al, 'O'
    jne .method_identify_not_found

    mov al, [request_buf + 0x02]
    cmp al, 'N'
    jne .method_identify_not_found

    mov al, [request_buf + 0x03]
    cmp al, 'N'
    jne .method_identify_not_found

    mov al, [request_buf + 0x04]
    cmp al, 'E'
    jne .method_identify_not_found

    mov al, [request_buf + 0x05]
    cmp al, 'C'
    jne .method_identify_not_found

    mov al, [request_buf + 0x06]
    cmp al, 'T'
    jne .method_identify_not_found

    mov al, [request_buf + 0x07]
    cmp al, ' '
    jne .method_identify_not_found

    mov byte[method], 0x07

ret

    ; --------------------------
    ;     NOT FOUND METHOD
    ; --------------------------
    .method_identify_not_found:
    mov byte[method], -1

ret