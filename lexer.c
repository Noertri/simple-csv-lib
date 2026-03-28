#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include "include/csv_internal.h"


Token make_str_tok(char *buffer, int *position, const CSV_OPTS options)
{
    Token token;
    token.type = STR;
    int i = *position;
    int k = 0;
    size_t capacity = 256;

    char* output = malloc(capacity*sizeof(char));
    
    if (!output) {
        token.value = NULL;
        return token;
    }

    while (buffer[i] != options.delimiter && buffer[i] != '\0')
    {
        if (k >= capacity-1) {
            capacity *= 2;
            char *temp = realloc(output, capacity*sizeof(char));

            if (!temp) {
                free(output);
                token.value = NULL;
                return token;
            }

            output = temp;
        }

        if (buffer[i] == '\r' || buffer[i] == '\n') {
            i++;
            continue;
        } else {
            output[k] = buffer[i];
            i++;
            k++;
        }
    } 
    
    output[k] = '\0';
    *position = i;

    token.value = output;
    
    return token;
}

Token make_quote_str_tok(char *buffer, int *position, const CSV_OPTS options)
{
    Token token;
    token.type = QUOTE_STR;
    int i = *position;
    i += 1;
    int k = 0;
    size_t capacity = 256;
    char *output = malloc(capacity*sizeof(char));
    
    if(!output) {
        token.value = NULL;
        return token;
    }

    while (buffer[i] != options.quote && buffer[i] != '\0')
    {
        if (k >= capacity-1) {
            capacity *= 2;
            char *temp = realloc(output, capacity*sizeof(char));

            if (!temp) {
                free(output);
                token.value = NULL;
                return token;
            }

            output = temp;
        }

        if (buffer[i] == options.escape) {
            i++;

            if (buffer[i] == 'n') {
                output[k] = '\n';
                k++;
            } else if (buffer[i] == 't') {
                output[k] = '\t';
                k++;
            } else if (buffer[i] == '\\') {
                output[k] = '\\';
                k++;
            } else if (buffer[i] =='"') {
                output[k] = '"';
                k++;
            } else if (buffer[i] == '\'') {
                output[k] = '\'';
                k++;
            } else if (buffer[i] == 'r') {
                output[k] = '\r';
                k++;
            }
            
            i++;
        } else {
            output[k] = buffer[i];
            i++;
            k++;
        }
    }
    
    if (buffer[i] == options.quote) {
        i++;
    }
    output[k] = '\0';
    *position = i;

    token.value = output;

    return token;
}


void append(TOKEN_H array, int index, Token value) {
    array[index] = value;
}


TOKENS tokenizer(char *buffer, const CSV_OPTS options)
{
    int capacity = 10;
    TOKEN_H tokens = malloc(capacity*sizeof(Token));
    
    if (!tokens) {
        TOKENS empty = {0, NULL};
        return empty;
    }

    int i = 0;
    int pos_char = 0;
    
    while(buffer[pos_char] != '\0') 
    {
        if (i >= capacity) {
            capacity *= 2;
            TOKEN_H new_tokens = realloc(tokens,\
                    capacity*sizeof(Token));
            
            if (!new_tokens) {
                for (int j = 0; j < i; j++) {
                    if (tokens[j].value != NULL) {
                        free(tokens[j].value);
                    }
                }

                free(tokens);
                TOKENS empty = {0, NULL};
                return empty;
            }

            tokens = new_tokens;
        }

        if (buffer[pos_char] == '\n' ||\
            buffer[pos_char] == '\r') {
            pos_char++;
            continue;
        } else if (buffer[pos_char] == options.delimiter) {
            Token delim_tok;
            delim_tok.type = DELIM;
            delim_tok.value = NULL;
            append(tokens, i, delim_tok);
            pos_char++;
            i++;
        } else if (buffer[pos_char] == options.quote) {
            Token quote_str_tok = make_quote_str_tok(buffer, &pos_char, options);
            append(tokens, i, quote_str_tok);
            i++;
        } else {
            Token str_tok = make_str_tok(buffer, &pos_char, options);
            append(tokens, i, str_tok);
            i++;
        }
    }

    TOKENS arr_tokens;
    arr_tokens.len = i;
    arr_tokens.data = tokens;
    return arr_tokens;
}


