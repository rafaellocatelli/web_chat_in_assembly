%include 'data/dt_system_constants.inc'
%include 'data/dt_info.inc'
%include 'data/dt_geral.inc'

section .text
    global _start

_start:

    mov word[port], 6969
    %include 'module/server_process.inc'
    %include 'module/controller_routes.inc'
    %include 'module/process_exit.inc'

