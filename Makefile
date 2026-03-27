run: build
	./main

build: main.c csv.c lexer.c
	${CC} -g -oo -Iinclude main.c csv.c lexer.c -o main

