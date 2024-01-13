# web_chat_in_assembly

Web Application in NASM (Netwide Assembler) for Linux x86_64

# Target

- no libc, pure syscalls
- no high level assembly

# Quick Start

01. Install [NASM](https://www.nasm.us/)

02. Instal [GNU Binutils for Ubuntu](https://packages.ubuntu.com/focal/binutils)

03. Compile and execute the app
```console
$ nasm main.asm -f elf64 -o main.o
$ ld main.o -m elf_x86_64 -o main
$ ./main
```

Start
  :
  :
  +--- orquestrador da conexão
  :                 :   :                 
  :                 :   +-- cria socket
  :                 :
  :                 +-- feedback da conexão
  :
  +--- orquestrador de rotas
              :  :
              :  +-- identifica o método
              :
              +-- identifica a rota