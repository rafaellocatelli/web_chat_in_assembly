section .text

; --------------------------------------------------------------------------------------------------------
; Process exit
; --------------------------------------------------------------------------------------------------------
; Description:  syscall exit terminates the calling process "immediately".
;               
; - rax: syscall NR
; - rdi: the value status
;           0 == exit normally
;           0 != exit error
;
; Return value: These functions do not return.
; --------------------------------------------------------------------------------------------------------

.process_exit_error:
    ; syscall exit 
    mov rax, SYSCALL_EXIT
    mov rdi, PROCESS_EXIT_ERROR
    syscall

.process_exit_success:
    ; print information about closing web server 
    mov rax, SYSCALL_WRITE                  ; syscall:  write
    mov rdi, STANDARD_OUTPUT_FD             ; arg0:     Standard Output 
    mov rsi, info_close                     ; arg1:     char *buf
    mov rdx, info_close_len                 ; arg2:     buf length     
    syscall

    ; syscall exit
    mov rax, SYSCALL_EXIT
    mov rdi, PROCESS_EXIT_NORMALLY
    syscall
