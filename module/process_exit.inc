section .text

; --------------------------------------------------------------------------------------------------------
; Close system call
; --------------------------------------------------------------------------------------------------------
; Descrição:    Syscall close fecha um descritor de arquivo, para que ele não se refira mais a qualquer 
;               arquivo e pode ser reutilizado.
;
; Declarando a syscall:
;               - rax:
;                   syscall number
;
;               - rdi:
;                   Informamos o socket file descriptor
;
; --------------------------------------------------------------------------------------------------------

.close_fd_befor_process_exit_success:
    mov rax, SYSCALL_CLOSE
    mov rdi, [new_socket_fd]
    syscall

    mov rax, SYSCALL_CLOSE
    mov rdi, [server_fd]
    syscall

    jmp .process_exit_success

.close_fd_befor_process_exit_error:
    mov rax, SYSCALL_CLOSE
    mov rdi, [server_fd]
    syscall

; --------------------------------------------------------------------------------------------------------
; Exit system call
; --------------------------------------------------------------------------------------------------------
; Descrição:    Syscall exit informando o status de saída (mais conhecido como código de saída). 
;
;               Há códigos de saída com significados convencionados, mas, em nosso programa, 
;               por enquanto, não fazemos distinção de erros. Representamos 0 como sucesso e todo erro
;               resulta em 1.
;
; Declarando a syscall:
;               - rax:
;                   syscall number
;
;               - rdi:
;                   Informamos o status de saída
;
; --------------------------------------------------------------------------------------------------------

.process_exit_error:
    mov rax, SYSCALL_EXIT
    mov rdi, PROCESS_EXIT_ERROR
    syscall

.process_exit_success:
    mov rax, SYSCALL_WRITE
    mov rdi, STANDARD_OUTPUT_FD
    mov rsi, info_close
    mov rdx, info_close_len   
    syscall

    mov rax, SYSCALL_EXIT
    mov rdi, PROCESS_EXIT_NORMALLY
    syscall