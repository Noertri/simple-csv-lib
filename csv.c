#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include "include/csv.h"
#include "include/csv_internal.h"


int destroy_tokens(TOKENS tokens)
{
    int capacity = tokens.len;

    for(int i = 0; i < capacity; i++) {
        if (tokens.data[i].value != NULL) {
            free(tokens.data[i].value);
            tokens.data[i].value = NULL;
        }
    }
    
    if (tokens.data != NULL) {
        free(tokens.data);
        tokens.data = NULL;
    }

    return 0;
}


typedef struct __parser_result {
    int len;
    CHAR2D_H values;
} ParserResult;


int deep_copy(CHAR2D_H dest, int dest_index, char *src)
{
    if (src != NULL) {
        size_t n = strlen(src)+1;
        dest[dest_index] = malloc(n*sizeof(char));
        memcpy(dest[dest_index], src, n);
    }

    return 0;
}


ParserResult parser(TOKENS tokens)
{
    int capacity = ((tokens.len+1)/2);
    TOKEN_H tokens_data = tokens.data;
    CHAR2D_H token_values = malloc(capacity*sizeof(char*));
    ParserResult result;

    int i = 0;
    int j = 0;

    while (i < tokens.len) {
        if (tokens_data[i].type == STR) {
            deep_copy(token_values, j, tokens_data[i].value);
            i++;
            j++;
        } else if (tokens_data[i].type == QUOTE_STR) {
            deep_copy(token_values, j, tokens_data[i].value);
            i++;
            j++;
        } else {
            i++;
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
        ParserResult parser_results = parser(tokens);
        
        int col_cap = parser_results.len;
        str2d[i] = malloc(col_cap*sizeof(char*));

        j = 0;

        for(int k = 0; k < parser_results.len; k++) {
            size_t n = strlen(parser_results.values[k])+1;
            str2d[i][j] = malloc(n*sizeof(char));
            memcpy(str2d[i][j], parser_results.values[k], n);
            j++;
        }

        i++;
        
        for (int k = 0; k < parser_results.len; k++) {
            free(parser_results.values[k]);
        }

    }

    result.rows = i;
    result.columns = j;
    result.str2d = str2d;

    return result;
}


