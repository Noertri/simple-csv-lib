run: build
	./main

build: main.c csv.c lexer.c
	${CC} main.c csv.c lexer.c -o main

