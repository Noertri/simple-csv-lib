#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include "include/csv.h"
#include "include/lexer.h"


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


CSV csv_reader(FILE *src) {
    CSV result;
    int row_cap = 10;
    CHAR3D_H str2d = malloc(row_cap*sizeof(CHAR2D_H));    
    int i = 0;
    int j;
    while(1) {
        if (i >= row_cap) {
            row_cap *= 2;
            str2d = realloc(str2d, row_cap*sizeof(CHAR2D_H));
        }

        char buffer[256];
        char *status = fgets(buffer, 256, src);

        if (status == NULL) {
            break;
        }

        TOKENS tokens = tokenizer(buffer, ',');

        int tokens_len = tokens.len;
        TOKEN_H tokens_data = tokens.data;

        int col_cap = ((tokens_len+1)/2);
        str2d[i] = malloc(col_cap*sizeof(char*));

        j = 0;
        int k = 0;

        while(k < tokens_len) {
            if (tokens_data[k].type == STR) {
                str2d[i][j] = tokens_data[k].value;
                j++;
                k++;
            } else if (tokens_data[k].type == QUOTE_STR) {
                str2d[i][j] = tokens_data[k].value;
                j++;
                k++;
            } else {
                k++;
                continue;
            }
        }
        
        i++;
    }

    result.rows = i;
    result.columns = j;
    result.str2d = str2d;

    return result;
}


