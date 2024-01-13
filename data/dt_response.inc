section .data

ok_response_one:   
    db "HTTP/1.1 200 OK", 13, 10
    db "Content-Type: text/html; charset=utf-8", 13, 10
    db "Connection: close", 13, 10
    db 13, 10
    db 0
ok_response_one_len: equ $ - ok_response_one

; "Keep-Alive: 100"
; "Connection: keep-alive"