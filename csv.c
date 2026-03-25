#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include "include/csv.h"
#include "include/lexer.h"


typedef char** CHAR2D_H ;
typedef char*** CHAR3D_H;


int destroy_tokens(TOKENS tokens)
{
    int capacity = tokens.len;

    for(int i = 0; i < capacity; i++)
    {
        free(tokens.data[i].value);
        tokens.data[i].value = NULL;
    }

    free(tokens.data);
    tokens.data = NULL;

    return 0;
}


typedef struct __parser_result {
    int len;
    CHAR2D_H values;
} ParserResult;


ParserResult parser(TOKENS tokens)
{
    int capacity = tokens.len;
    TOKEN_H tokens_data = tokens.data;
    CHAR2D_H token_values = malloc(capacity*sizeof(char*));
    ParserResult result;

    int i = 0;
    int j = 0;

    while (i < capacity) {
        switch (tokens_data[i].type) {
            case STR:
                token_values[j] = tokens_data[i].value;
                i++;
                j++;
                break;
            case QUOTE_STR:
                token_values[j] = tokens_data[i].value;
                i++;
                j++;
                break;
            default:
                continue;
        }
    }

    destroy_tokens(tokens);

    result.len = j;
    result.values = token_values;
    return result;
}


void csv_reader(FILE *src) {
    for (int i = 0; i < 101; i++) {
        char buffer[256];
        fgets(buffer, 256, src);
        TOKENS str_tokens = tokenizer(buffer, ',');
        parser(str_tokens);
    }
}


