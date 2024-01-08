section .data

; --------------------------------------------------------------------------------------------------------
; Linux x86_64 (64-bit) System Call Table
; --------------------------------------------------------------------------------------------------------
; Descrição:
;               Nossa aplicação é executada na camada do usuário e precisamos do sistema operacional
;               para executar algumas ações.
;
;               Syscall é uma pequena função de biblioteca que invoca a chamada do sistema (syscall) 
;               cuja interface em linguagem assembly possui o número especificado com 
;               os argumentos especificados.
;
; Pontos de atenção:
;               Idealmente deveriamos evitar utilizar syscall e optar por utilizar a libc, porém, 
;               este projeto foi desenvolvido com a intenção de se aprofundar nos fundamentos da 
;               programação, ignorando propositalmente as boas práticas.
;
; Veja também:
;               Artigo do Wikipédia sobre as camadas
;               https://pt.wikipedia.org/wiki/Sistema_Operacional_em_Camadas
;
;               Tabela completa com os system call numbers (syscall NR)
;               https://chromium.googlesource.com/chromiumos/docs/+/master/constants/syscalls.md 
;
; --------------------------------------------------------------------------------------------------------

    SYSCALL_READ                equ     0x00
    SYSCALL_WRITE               equ     0x01
    SYSCALL_CLOSE               equ     0x03
    SYSCALL_SOCKET              equ     0x29
    SYSCALL_ACCEPT              equ     0x2b
    SYSCALL_BIND                equ     0x31
    SYSCALL_LISTEN              equ     0x32
    SYSCALL_SETSOCKOPT          equ     0x36
    SYSCALL_EXIT                equ     0x3c

; --------------------------------------------------------------------------------------------------------
; File descriptor values
; --------------------------------------------------------------------------------------------------------
; Descrição:
;               Para o Linux, tudo é considerado como um arquivo. E o File Descriptor é uma forma de haver
;               comunicação entre os arquivos.
;
; Veja também:
;               Artigo do Wikipédia sobre File Descriptor
;               https://en.wikipedia.org/wiki/File_descriptor
;
; --------------------------------------------------------------------------------------------------------

    STANDARD_INPUT_FD           equ     0x00
    STANDARD_OUTPUT_FD          equ     0x01
    STANDARD_ERROR_FD           equ     0x02

; --------------------------------------------------------------------------------------------------------
; Process exit codes
; --------------------------------------------------------------------------------------------------------
; Descrição:
;               No Linux, para indicar o fim da execução de um programa, usamos syscall exit informando
;               o status de saída (mais conhecido como código de saída).
;
;               Os códigos de saída variam de 0 a 255, com 0 representando sucesso 
;               e 1-255 indicando várias condições de falha.
;
;               Há códigos de saída com significados convencionados, mas, em nosso programa, 
;               por enquanto, não fazemos distinção de erros. Representamos 0 como sucesso e todo erro
;               resulta em 1.
;
;               Para o objetivo deste projeto, o código de saída que representa o erro é indiferente, você
;               pode alterá-lo para qualquer número entre 1-255.
; 
; Veja também:
;               Artigo do Wikipédia sobre Exit status
;               https://en.wikipedia.org/wiki/Exit_status
;
; --------------------------------------------------------------------------------------------------------

    PROCESS_EXIT_NORMALLY       equ     0x00
    PROCESS_EXIT_ERROR          equ     0x01