section .text

; --------------------------------------------------------------------------------------------------------
; Feedback sobre o início do server web
; --------------------------------------------------------------------------------------------------------
; Descrição:    Apenas imprimimos no console do server uma mensagem como feedback que o processo 
;               foi iniciado.
;               
; --------------------------------------------------------------------------------------------------------

    mov rax, SYSCALL_WRITE
    mov rdi, STANDARD_OUTPUT_FD
    mov rsi, info_start
    mov rdx, info_start_len
    syscall

; --------------------------------------------------------------------------------------------------------
; Socket system call
; --------------------------------------------------------------------------------------------------------
; Descrição:    Syscall socket cria um endpoint para comunicação e retorna um file descriptor.
;               Esse file descriptor aponta indiretamente para o endpoint.
;               Ou seja, sempre que quisermos configurar o endpoint ou estabelecer uma comunicação, faremos
;               atráves desse file descriptor. 
;
;               Se a criação do endpoint for feita com sucesso, imprimos uma mensagem informando sobre o 
;               sucesso e armazemos o file descriptor na memória.
;
;               Se não obtivermos sucesso na criação do endpoint, imprimos uma mensagem informando sobre o 
;               insucesso e direcionamento para o fim do programa.
;
; Declarando a syscall:
;               - rax:
;                   syscall number
;
;               - rdi:
;                   Definimos a família de protocolo AF_INET (IPv4 Internet protocols).
;
;                   Os soquetes da família de endereços AF_INET podem ser orientados à conexão (tipo SOCK_STREAM)
;                   ou sem conexão (tipo SOCK_DGRAM).
;
;               - rsi:
;                   SOCK_STREAM fornece fluxos de bytes sequenciais, confiáveis, bidirecionais e baseados em conexão.
;
;                   Os soquetes AF_INET orientados à conexão (tipo SOCK_STREAM) usam o Transmission Control Protocol (TCP)
;                   como protocolo de transporte.
;
;               - rdx:
;                   IPPROTO_IP é um protocolo fictício para TCP.
;
;                   O argumento rdx especifica um protocolo específico a ser usado com o socket.
;                   Nos casos onde há um único protocolo para suportar, o protocolo pode ser especificado
;                   como 0.
;
;                   A constante IPPROTO_IP tem valor 0.
;
;                   Quando o tipo de socket for SOCK_STREAM e a família for AF_INET, o protocolo será 
;                   automaticamente TCP (exatamente o mesmo como se você tivesse usado IPPROTO_TCP) por
;                   ser o único protocolo para suportar.
;
; --------------------------------------------------------------------------------------------------------

    mov rax, SYSCALL_SOCKET
    mov rdi, AF_INET
    mov rsi, SOCK_STREAM
    mov rdx, IPPROTO_IP
    syscall

    cmp rax, 0x00
    jge .socket_success

.socket_error:
    mov rax, SYSCALL_WRITE
    mov rdi, STANDARD_OUTPUT_FD
    mov rsi, info_socket
    mov rdx, info_socket_len
    syscall

    mov rax, SYSCALL_WRITE
    mov rdi, STANDARD_OUTPUT_FD
    mov rsi, info_fail
    mov rdx, info_fail_len
    syscall

    jmp .process_exit_error

.socket_success:
    mov qword[server_fd], rax

    mov rax, SYSCALL_WRITE
    mov rdi, STANDARD_OUTPUT_FD
    mov rsi, info_socket
    mov rdx, info_socket_len
    syscall
    
    mov rax, SYSCALL_WRITE
    mov rdi, STANDARD_OUTPUT_FD
    mov rsi, info_success
    mov rdx, info_success_len
    syscall

; --------------------------------------------------------------------------------------------------------
; Invertendo os bytes da porta
; --------------------------------------------------------------------------------------------------------
; Descrição:    Intel x86_64 é Little Endian e a network byte order é Big Endian.
;               
;               O número da porta da conexão fica salvo em um espaço de memória de 16-bit.
;               A instrução "rol" inverte os primeiros 8-bit com os últimos 8-bit.
;
;               Em nossa aplicação, definimos que a conexão será na porta "6969".
;               Em hexadecimal, o valor "6969" fica "1b39" ao inverter os bytes ficará "391b".
;
; Veja também:
;               O artigo do Wikipédia sobre ordenação dos bytes
;               https://pt.wikipedia.org/wiki/Extremidade_(ordena%C3%A7%C3%A3o)
;
; --------------------------------------------------------------------------------------------------------

    mov ax, [port]
    rol ax, 0x08
    mov word[port_htons], ax   

; --------------------------------------------------------------------------------------------------------
; Definindo a estrutuda de dados do socket
; --------------------------------------------------------------------------------------------------------
; Descrição:    Nós definiremos uma estrutura para conexão ipv4 (o que basicamente todo mundo ainda usa).
;               
;               socket address structure ipv4:
;
;               name        - size              - description
;               ------------+-------------------+-------------------
;               sin_family  - 2 bytes (16-bits) - address family
;               sin_port    - 2 bytes (16-bits) - port in network byte order
;               sin_addr    - 4 bytes (32-bits) - address in network byte order
;               sin_padding - 8 bytes (64-bits) - padding
;
;               Total de 16 bytes
;
;               sin_family:
;                   Especificaremos AF_INET.
;                   AF_INET é uma família de endereços usada para designar o tipo de endereço com o qual o 
;                   socket pode se comunicar (IPv4 Internet protocols).
;
;               sin_port:
;                   Especificaremos o port_htons que consta a porta na ordem correta de bytes. 
;
;               sin_addr:
;                   Manteremos todos os bytes zerados.
;                   INADDR_ANY (0.0.0.0) significa qualquer endereço para ligação;
;
;               sin_padding:
;                   A melhor explicação que achei diz que provavelmente o foi adicionado na tentativa
;                   de suportar futuros formatos de endereço de rede que possam ser maiores que os 
;                   endereços IPv4.
; 
;                   Ironicamente, isso foi insuficiente para suportar endereços IPv6, que têm 16 bytes 
;                   de comprimento (o dobro do tamanho do sin_padding).
;               
; --------------------------------------------------------------------------------------------------------

    mov word[server_addr.sin_family], AF_INET

    mov ax, [port_htons]
    mov word[server_addr.sin_port], ax

; --------------------------------------------------------------------------------------------------------
; Bind system call
; --------------------------------------------------------------------------------------------------------
; Descrição:    Syscall bind vincula socket address structure ipv4 ao socket file descriptor que criamos 
;               com o syscall socket.
;
;               Essa operação precisa acontecer ANTES do SOCK_STREAM possa receber conexões.
;               Em outras palavras, antes de acionarmos a syscall accept.
;
; Declarando a syscall:
;               - rax:
;                   syscall number
;
;               - rdi:
;                   Informamos o socket file descriptor criado pelo syscall socket
;
;               - rsi:
;                   Informamos o socket address structure que desejamos vincular ao socket file descriptor
;
;               - rdx:
;                   Informamos o tamanho em bytes do nosso socket address structure.
;
;                   Esta informação é necessária pois existem socket address structure de diferentes tamanhos.
;                   Por exemplo, o socket address structure ipv6 tem 28 bytes de tamanho e o nosso ipv4 tem 
;                   16 bytes de tamanho.
;
; --------------------------------------------------------------------------------------------------------

    mov rax, SYSCALL_BIND
    mov rdi, [server_fd]
    mov rsi, server_addr.sin_family
    mov rdx, SOCKET_ADDRESS_STRUCT_SIZE
    syscall

    cmp rax, 0x00
    je .bind_success

.bind_error:
    mov rax, SYSCALL_WRITE
    mov rdi, STANDARD_OUTPUT_FD
    mov rsi, info_bind
    mov rdx, info_bind_len
    syscall

    mov rax, SYSCALL_WRITE
    mov rdi, STANDARD_OUTPUT_FD
    mov rsi, info_fail
    mov rdx, info_fail_len
    syscall

    jmp .close_fd_befor_process_exit_error

.bind_success:
    mov rax, SYSCALL_WRITE
    mov rdi, STANDARD_OUTPUT_FD
    mov rsi, info_bind
    mov rdx, info_bind_len
    syscall
    
    mov rax, SYSCALL_WRITE
    mov rdi, STANDARD_OUTPUT_FD
    mov rsi, info_success
    mov rdx, info_success_len
    syscall

; --------------------------------------------------------------------------------------------------------
; Listen system call
; --------------------------------------------------------------------------------------------------------
; Descrição:    Syscall listen torna o socket como socket passivo, ou seja, como um socket que será usado
;               para aceitar solicitações de conexão. 
;
;               Esta operação precisa ser feita antes da syscall accept.
;
; Declarando a syscall:
;               - rax:
;                   syscall number
;
;               - rdi:
;                   Informamos o socket file descriptor, pois é ele quem faz referência ao nosso socket
;
;               - rsi:
;                   Informamos o máximo de conexões que poderão ficar na fila para "serem atendidas".
;
;                   Pelo o que eu entendi, nosso programa é single thread, ou seja, atende uma conexão por vez.
;                   O argumento de rsi definirá quantas conexões poderão ficar na fila de espera.
;                   Em caso de superlotação na "fila de espera", as próximas conexões serão negadas até
;                   ter uma nova vaga na "fila de espera".
;
; --------------------------------------------------------------------------------------------------------

    mov rax, SYSCALL_LISTEN
    mov rdi, [server_fd]
    mov rsi, MAXIMUM_CONNECTIONS
    syscall

    cmp rax, 0x00
    je .listen_success

.listen_error:
    mov rax, SYSCALL_WRITE
    mov rdi, STANDARD_OUTPUT_FD
    mov rsi, info_listen
    mov rdx, info_listen_len
    syscall

    mov rax, SYSCALL_WRITE
    mov rdi, STANDARD_OUTPUT_FD
    mov rsi, info_fail
    mov rdx, info_fail_len
    syscall

    jmp .close_fd_befor_process_exit_error

.listen_success:
    mov rax, SYSCALL_WRITE
    mov rdi, STANDARD_OUTPUT_FD
    mov rsi, info_listen
    mov rdx, info_listen_len
    syscall

    mov rax, SYSCALL_WRITE
    mov rdi, STANDARD_OUTPUT_FD
    mov rsi, info_success
    mov rdx, info_success_len
    syscall

; --------------------------------------------------------------------------------------------------------
; Feedback sobre o server web estar disponível
; --------------------------------------------------------------------------------------------------------
; Descrição:    Apenas imprimimos no console do server uma mensagem informando que o server web foi iniciado.
;               
; --------------------------------------------------------------------------------------------------------

    mov rax, SYSCALL_WRITE
    mov rdi, STANDARD_OUTPUT_FD
    mov rsi, info_accept_success
    mov rdx, info_accept_success_len
    syscall

; --------------------------------------------------------------------------------------------------------
; Accept system call
; --------------------------------------------------------------------------------------------------------
; Descrição:    Syscall accept é usada com socket baseado em conexões do tipo SOCK_STREAM (nosso caso) ou
;               SOCK_SEQPACKET.
;
;               De acordo com o manual, o syscall accept basicamente:
;               1. Extrai a primeira solicitação de conexão na "fila de espera" do socket;
;               2. Cria um novo socket para essa conexão 
;               3. E nos informa o file descriptor desse novo socket
;
;               Nesse novo socket conterá o HTTP request do cliente.
;
; Declarando a syscall:
;               - rax:
;                   syscall number
;
;               - rdi:
;                   Informamos o socket file descriptor criado por nós
;
;               - rsi:
;                   Apontamos o endereço do socket address structure ipv4 reservado para essas conexões.
;
;               - rdx:
;                   Informamos o tamanho do socket address structure ipv4 reservado para essas conexões.
;
; --------------------------------------------------------------------------------------------------------

    mov rax, SYSCALL_ACCEPT
    mov rdi, [server_fd]
    mov rsi, client_addr.sin_family
    mov rdx, client_addr_len
    syscall

    cmp rax, 0x00
    jge .accept_success

.accept_error:
    mov rax, SYSCALL_WRITE
    mov rdi, STANDARD_OUTPUT_FD
    mov rsi, info_accept_error
    mov rdx, info_accept_error_len
    syscall

    jmp .close_fd_befor_process_exit_error

.accept_success:
    mov qword[new_socket_fd], rax