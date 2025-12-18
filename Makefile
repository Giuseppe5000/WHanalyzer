CC=gcc
CFLAGS=-Wall -Wextra -std=c99 -pedantic -Werror=vla

all: main

main: main.c src/lexer.c src/utils.c src/parser.c
	$(CC) $(CFLAGS) main.c src/lexer.c src/utils.c src/parser.c -o main
