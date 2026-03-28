#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include "include/csv.h"
#include "include/csv_internal.h"


int destroy_tokens(TOKENS tokens)
{
    int capacity = tokens.len;

    for(int i = 0; i < capacity; i++) {
        if (tokens.data[i].value != NULL) {
            free(tokens.data[i].value);
        }
    }
    
    if (tokens.data != NULL) {
        free(tokens.data);
        tokens.data = NULL;
    }

    return 0;
}


typedef struct __csv_row {
    int len;
    CHAR2D_H values;
} CSVRow;


CSVRow parser(TOKENS tokens)
{
    int capacity = ((tokens.len+1)/2);
    TOKEN_H tokens_data = tokens.data;
    CHAR2D_H token_values = malloc(capacity*sizeof(char*));
    CSVRow result;

    int i;
    int j;

    i = j = 0;

    while (i < tokens.len) {
        if (tokens_data[i].type == STR) {
            token_values[j] = tokens_data[i].value;
            tokens_data[i].value = NULL;
            i++;
            j++;
        } else if (tokens_data[i].type == QUOTE_STR) {
            token_values[j] = tokens_data[i].value;
            tokens_data[i].value = NULL;
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


static void cleanup_on_error(CHAR3D_H arr, int process_row, int process_col)
{
    for (int i = 0; i < process_row; i++) {
        for (int j = 0; j < process_col; j++) {
            free(arr[i][j]);
            arr[i][j] = NULL;
        }
    }

    free(arr);
    arr = NULL;
}


CSV csv_reader(FILE *src, CSV_OPTS options) {
    CSV result = {0};
    int row_cap = 10;
    CHAR3D_H records = malloc(row_cap*sizeof(CHAR2D_H));    
    
    if (!records) {
        return result;
    }

    int i = 0;
    int j;
    while(1) {
        if (i >= row_cap) {
            row_cap *= 2;
            CHAR3D_H new_records = realloc(records, row_cap*sizeof(CHAR2D_H));

            if (!new_records) {
                cleanup_on_error(records, i, j);
                return result;
            }

            records = new_records;
        }

        char buffer[256];
        char *status = fgets(buffer, 256, src);

        if (status == NULL) {
            break;
        }

        TOKENS tokens = tokenizer(buffer, ',');
        CSVRow row = parser(tokens);
        
        int col_cap = row.len;
        records[i] = malloc(col_cap*sizeof(char*));
        
        if (!records[i]) {
            cleanup_on_error(records, i, j);
            return result;
        }

        j = 0;

        for(int k = 0; k < row.len; k++) {
            records[i][j] = row.values[k];
            row.values[k] = NULL;
            j++;
        }

        i++;
        
        free(row.values);
        row.values = NULL;
    }

    result.rows = i;
    result.columns = j;
    result.records = records;

    return result;
}


