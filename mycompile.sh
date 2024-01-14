#!/bin/bash
nasm main.asm -f elf64 -o main.o && ld main.o -m elf_x86_64 -o main && clear && ./main