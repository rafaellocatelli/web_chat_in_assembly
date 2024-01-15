%include 'include/main.inc'

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

