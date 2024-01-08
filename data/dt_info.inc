section .data

    info_success                db      `\033[01;32mSUCCESS\033[00m`, 0x0a
    info_success_len            equ     $ - info_success

    info_fail                   db      `\033[01;31mFAIL   \033[00m`, 0x0a
    info_fail_len               equ     $ - info_fail
    
    info_start                  db      'Starting web server...', 0x0a
    info_start_len              equ     $ - info_start

    info_socket                 db      `\033[01mINFO:\033[00m socket `
    info_socket_len             equ     $ - info_socket

    info_bind                   db      `\033[01mINFO:\033[00m bind   `
    info_bind_len               equ     $ - info_bind

    info_listen                 db      `\033[01mINFO:\033[00m listen `
    info_listen_len             equ     $ - info_listen


    info_accept_error           db      `\033[01mINFO:\033[00m Accept `
                                db      `\033[01;31mFAIL   \033[00m`, 0x0a
    info_accept_error_len       equ     $ - info_accept_error


    info_accept_success         db      0x0a, `Web Server \033[32mon\033[00m`, 0x0a
                                db      'connect on http://127.0.0.1:6969 or http://localhost:6969', 0x0a, 0x0a
    info_accept_success_len     equ     $ - info_accept_success


    info_close                  db      0x0a, 'Closing web server...', 0x0a
                                db      `Web Server \033[31moff\033[00m`, 0x0a
    info_close_len              equ     $ - info_close
