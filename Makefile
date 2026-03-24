run: build
	./main


build: main.c csv.c
	${CC} main.c csv.c -o main

