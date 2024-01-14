%include 'include/main.inc'

section .data

; \033[0;36m - Cyan
; \033[1;36m - Cyan Bold
; \033[1;31m - Red Bold 
; \033[0m    - Reset

    rt_get:
        db `\033[0;36m┃      \033[0mGET     \033[0;36m┃\033[0m`
    rt_get_len: equ $ - rt_get

    rt_put:
        db `\033[0;36m┃      \033[0mPUT     \033[0;36m┃\033[0m`
    rt_put_len: equ $ - rt_put

    rt_post:    
        db `\033[0;36m┃     \033[0mPOST     \033[0;36m┃\033[0m`
    rt_post_len: equ $ - rt_post

    rt_delete:
        db `\033[0;36m┃    \033[0mDELETE    \033[0;36m┃\033[0m`
    rt_delete_len: equ $ - rt_delete

    rt_head:
        db `\033[0;36m┃     \033[0mHEAD     \033[0;36m┃\033[0m`
    rt_head_len: equ $ - rt_head

    rt_options:
        db `\033[0;36m┃    \033[0mOPTIONS   \033[0;36m┃\033[0m`
    rt_options_len: equ $ - rt_options

    rt_connect:
        db `\033[0;36m┃    \033[0mCONNECT   \033[0;36m┃\033[0m`
    rt_connect_len: equ $ - rt_connect

    rt_trace:
        db `\033[0;36m┃     \033[0mTRACE    \033[0;36m┃\033[0m`
    rt_trace_len: equ $ - rt_trace

    rt_not_found:
        db `\033[0;36m┃ ✕ \033[0mnot found  \033[0;36m┃\033[0m`
    rt_not_found_len: equ $ - rt_not_found

    rt_border_top:           
        db      0x0a, `\033[0;36m┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┳━━━━━━━━━━━━━━┳━━━━━━━━━━━┳━━━━━━━━━━━━━┓\033[0m`, 0x0a
        db      `\033[0;36m┃                                       \033[1;36mpath                                   \033[0;36m┃    \033[1;36mmethod    \033[0;36m┃ \033[1;36mparameter \033[0;36m┃   \033[1;36mstatus    \033[0;36m┃\033[0m`, 0x0a
        db      `\033[0;36m┣━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━╋━━━━━━━━━━━━━━╋━━━━━━━━━━━╋━━━━━━━━━━━━━┫\033[0m`, 0x0a
    rt_border_top_len: equ $ - rt_border_top


    rt_border_botton:
        db      `\033[0;36m┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┻━━━━━━━━━━━━━━┻━━━━━━━━━━━┻━━━━━━━━━━━━━┛\033[0m`, 0x0a, 0x0a
    rt_border_botton_len: equ $ - rt_border_botton


    rt_border_side:
        db      `\033[0;36m┃\033[0m`
    rt_border_side_len: equ $ - rt_border_side


    rt_space:
        db      ' '
    rt_space_len: equ $ - rt_space    

section .text
    global _start

_start:
    call _network_socket
    call _pull_connection
    call _method_identify
    call _route_identify
    call _request_trace

    .end_process:
    call _process_exit

_request_trace:
    mov rax, SYSCALL_WRITE
    mov rdi, STD_OUT
    mov rsi, rt_border_top
    mov rdx, rt_border_top_len
    syscall 

    mov rax, SYSCALL_WRITE
    mov rdi, STD_OUT
    mov rsi, rt_border_side
    mov rdx, rt_border_side_len
    syscall

    mov rax, SYSCALL_WRITE
    mov rdi, STD_OUT
    mov rsi, rt_space
    mov rdx, rt_space_len
    syscall 

    mov rax, SYSCALL_WRITE
    mov rdi, STD_OUT
    mov rsi, path
    mov rdx, [path_len]
    syscall

    mov r10, 0x4C
    sub r10, parameter_len
    mov r8, 0x01

    .rt_loop_path:
    mov rax, SYSCALL_WRITE
    mov rdi, STD_OUT
    mov rsi, rt_space
    mov rdx, rt_space_len
    syscall

    inc r8
    cmp r10, r8
    jl .rt_loop_path

ret
    





; NOT FOUND - -1
; GET       -  0
; POST      -  1
; PUT       -  2
; DELETE    -  3
; HEAD      -  4
; OPTIONS   -  5
; TRACE     -  6
; CONNECT   -  7


; ↺ loading..
; ✕ no
; ✓ yes
; \033[1;31m✕ not found

; ┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┳━━━━━━━━━━━━━━┳━━━━━━━━━━━┳━━━━━━━━━━━━━┓
; ┃                                       path                                   ┃    method    ┃ parameter ┃   status    ┃
; ┣━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━╋━━━━━━━━━━━━━━╋━━━━━━━━━━━╋━━━━━━━━━━━━━┫
; ┃ 0123456789012345678901234567890123456789012345678901234567890123456789012345 ┃12345678901234┃   ✓ yes   ┃ ↺ loading.. ┃
; ┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┻━━━━━━━━━━━━━━┻━━━━━━━━━━━┻━━━━━━━━━━━━━┛
;                                                                                                12345678901 1234567890123
; https://www.doc.ic.ac.uk/~svb/chars.html






voltando ao ritmo de estudos
surgiram trampos

surgiu B.O.:
1. família
2. trabalho

