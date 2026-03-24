run: build
	./main

build: main.c csv.c lexer.c
	${CC} -Iinclude main.c csv.c lexer.c -o main

