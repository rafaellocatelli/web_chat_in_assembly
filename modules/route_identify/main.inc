section .data 

    URL_MAX_LENGTH  equ     0x4b                    ; tamanho máximo da URL é 2,048 bytes mas definimos com 75 bytes

    path_len        dq      0x00
    parameter_len   dq      0x00

section .bss

    path            resb    URL_MAX_LENGTH
    parameter       resb    URL_MAX_LENGTH

section .text

_route_identify:
    ; request_buf_len   - tamanho do http request       source: 'pull_connection/main.inc'
    ; request_buf       - dados do http request         source: 'pull_connection/main.inc'

    xor rax, rax
    xor rdi, rdi
    xor rsi, rsi
    xor rdx, rdx 

    .route_identify_lookup_value:
    mov al, [request_buf + rdi]

    cmp rdi, URL_MAX_LENGTH
    jge .route_identify_error

    cmp al, 0x00
    je .route_identify_error

    cmp al, 0x0a
    je .route_identify_error

    cmp al, '/'
    je .route_identify_path

    inc rdi
    jmp .route_identify_lookup_value

    .route_identify_path:
    mov al, [request_buf + rdi]

    cmp al, 0x00
    je .route_identify_success

    cmp al, 0x0a
    je .route_identify_success

    cmp al, '?'
    je .route_identify_parameter

    cmp al, '#'
    je .route_identify_success

    cmp al, ' '
    je .route_identify_success

    mov byte[path + rsi], al
    inc rdi
    inc rsi

    cmp rdi, URL_MAX_LENGTH
    jge .route_identify_success 
    jmp .route_identify_path

    .route_identify_parameter:
    mov al, [request_buf + rdi]

    cmp al, 0x00
    je .route_identify_success

    cmp al, 0x0a
    je .route_identify_success

    cmp al, '#'
    je .route_identify_success

    cmp al, ' '
    je .route_identify_success

    mov byte[parameter + rdx], al
    inc rdi 
    inc rdx

    cmp rdi, URL_MAX_LENGTH
    jge .route_identify_success
    jmp .route_identify_parameter

    .route_identify_error:
    mov qword[path_len], 0x00
    mov qword[parameter_len], 0x00
    mov rax, -1
ret 

    .route_identify_success:
    mov qword[path_len], rsi
    mov qword[parameter_len], rdx
    mov rax, 1
ret