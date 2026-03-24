#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include "include/csv.h"
#include "include/lexer.h"


//typedef char** CHAR2D_H ;
typedef char*** CHAR3D_H;

CHAR3D_H parser(TOKEN_H tokens);


void csv_reader(FILE *src) {
    for (int i = 0; i < 101; i++) {
        char buffer[256];
        fgets(buffer, 256, src);
        TOKEN_H str_tokens = tokenizer(buffer, ',');

        printf("%-10s", str_tokens[0].value);
        printf("%-10s", str_tokens[2].value);
        printf("%s", str_tokens[8].value);
        printf("\n");
    }
}


