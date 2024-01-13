section .text

; --------------------------------------------------------------------------------------------------------
; Ler o HTTP Request da conexão
; --------------------------------------------------------------------------------------------------------
; Descrição:    Syscall read acessa o socket file descriptor referenciado pelo rdi e transfere toda 
;               informação para o buffer apontado em rsi. E retornará em rax o tamanho do HTTP Request
;               em bytes.
;
; Declarando a syscall:
;               - rax:
;                   syscall number
;
;               - rdi:
;                   Informamos o socket file descriptor da conexão
;
;               - rsi:
;                   Apontaremos o buffer onde deve ser armazenado toda mensagem do HTTP Request
;
;               - rdx:
;                   Informamos o tamanho total do buffer que apontamos para evitar buffer overflow.
;
; --------------------------------------------------------------------------------------------------------

    mov rax, SYSCALL_READ
    mov rdi, [new_socket_fd]
    mov rsi, request_buf
    mov rdx, REQUEST_BUFFER_SIZE  
    syscall

    cmp rax, 0x00
    jge .read_http_request_success

.read_http_request_error:
    jmp .close_fd_befor_process_exit_error

.read_http_request_success:
    mov qword[request_len], rax


; --------------------------------------------------------------------------------------------------------
; Imprime o HTTP Request
; --------------------------------------------------------------------------------------------------------
; Descrição:    Usamos o syscall write para imprimir o conteúdo do "request_buf" no Standard Output   
;
; --------------------------------------------------------------------------------------------------------

    mov rax, SYSCALL_WRITE
    mov rdi, STANDARD_OUTPUT_FD
    mov rsi, request_buf
    mov rdx, [request_len]   
    syscall

; --------------------------------------------------------------------------------------------------------
; Devolve uma HTTP Response
; --------------------------------------------------------------------------------------------------------
; Descrição:    Teste de dois response
;
; --------------------------------------------------------------------------------------------------------

; /* open-only flags */
; #define   O_RDONLY    0x0000      /* open for reading only */
; #define   O_WRONLY    0x0001      /* open for writing only */
; #define   O_RDWR      0x0002      /* open for reading and writing */
; #define   O_ACCMODE   0x0003      /* mask for above modes */

    mov rax, SYSCALL_OPEN
    mov rdi, name_html
    mov rsi, 0x0000                     ; O_RDONLY - open for reading only
    syscall

    cmp rax, 0x00
    jl .close_fd_befor_process_exit_error
    mov qword[open_fd], rax 

    mov rax, SYSCALL_READ
    mov rdi, [open_fd]
    mov rsi, response_buf
    mov rdx, REQUEST_BUFFER_SIZE
    syscall

    mov qword[response_len], rax

    ; header
    mov rax, SYSCALL_WRITE
    mov rdi, [new_socket_fd]
    mov rsi, ok_response_one
    mov rdx, ok_response_one_len
    syscall

    mov rax, SYSCALL_WRITE
    mov rdi, [new_socket_fd]
    mov rsi, response_buf
    mov rdx, response_len
    syscall