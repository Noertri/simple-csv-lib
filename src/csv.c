#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include "csv.h"
#include "csv_internal.h"


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


CSVRow parser(TOKENS tokens, char *null_value)
{
    CSVRow result = {0};
    int capacity = tokens.len;
    TOKEN_H tokens_data = tokens.data;
    CHAR2D_H token_values = malloc(capacity*sizeof(char*));

    if (!token_values) {
        perror("Failed to allocate memory!!!");
        token_values = NULL;
        return result;
    }

    int i;
    int j;

    i = j = 0;

    if (tokens_data[0].type == DELIM) {
        token_values[j] = NULL;
        j++;
    }
    
    while (i < tokens.len) {
        if (tokens_data[i].type == FIELD) {
            token_values[j] = tokens_data[i].value;
            tokens_data[i].value = NULL;
            i++;
            j++;
        } else if (tokens_data[i].type == DELIM) {
            if (tokens_data[i+1].type == DELIM) {
                token_values[j] = NULL;
                i++;
                j++;
            } else if (tokens_data[i+1].type == EOL) {
                token_values[j] = NULL;
                i++;
                j++;
            } else {
                i++;
            }
        } else {
            break;
        }
    }

    destroy_tokens(tokens);

    result.len = j;
    result.values = token_values;
    return result;
}


static void cleanup_on_error(RECORDS_H arr, int process_row)
{
    for (int i = 0; i < process_row; i++) {
        CSVRow row = arr[i];
        for (int j = 0; j < row.len; j++) {
            free(row.values[j]);
            row.values[j] = NULL;
        }
    }

    free(arr);
    arr = NULL;
}


CSV csv_reader(FILE *src, int row_cap, CSVOpts csv_options) {
    /*
        FILE *src               : Pointer to filestream
        size_t buffer_size      : Size of the buffer
        CSVOpts csv_options    : CSV related options (delimiter, escape character, quote character, etc)
    */

    if (!csv_options.delimiter) {
        csv_options.delimiter = ',';
    } 

    if (!csv_options.escape) {
        csv_options.escape = '\\';
    }

    if (!csv_options.quote) {
        csv_options.quote = '"';
    }

    if (!csv_options.buffer_size) {
        csv_options.buffer_size = 256;
    }

    CSV result = {0};
    //int row_cap = 100;
    RECORDS_H records = malloc(row_cap*sizeof(CSVRow)); 
    
    if (!records) {
        return result;
    }

    int i = 0;
    int j;
    while(i < row_cap) {
        /*if (i >= row_cap) {
            row_cap *= 10;
            records = realloc(records, row_cap*sizeof(CHAR2D_H));

            if (!records) {
                perror("Failed to reallocate memory!!!");
                cleanup_on_error(records, i);
                return result;
            }
        }*/

        char buffer[csv_options.buffer_size];
        char *status = fgets(buffer, csv_options.buffer_size, src);

        if (status == NULL) {
            break;
        }

        TOKENS tokens = tokenizer(buffer, csv_options);
        CSVRow row = parser(tokens, csv_options.null_value);
        records[i] = row;   
        i++;
        //destroy_tokens(tokens);
    }

    result.rows = i;
    result.records = records;

    return result;
}


int csv_destroy(CSV csv) {
    for (int i = 0; i < csv.rows; i++) {
        CSVRow row = csv.records[i];
        for (int j = 0; j < row.len; j++) {
            free(row.values[j]);
            row.values[j] = NULL;
        }
    }

    free(csv.records);
    csv.records = NULL;
    return 0;
}

