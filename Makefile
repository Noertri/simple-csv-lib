SRC=src
DEBUG=debug
INCLUDE=include
TESTS=tests

run: debug
	./${DEBUG}/test.out "samples/people.csv" | less

debug: ${TESTS}/test.c ${SRC}/csv.c ${SRC}/lexer.c
	${CC} -g -Iinclude ${TESTS}/test.c ${SRC}/csv.c ${SRC}/lexer.c -o ${DEBUG}/test.out

