run: build
	./main | less

build: main.c csv.c lexer.c
	${CC} -g -Iinclude main.c csv.c lexer.c -o main

