#include <stdlib.h>
#include "include/lexer.h"


char* make_str(char *buffer, int *position, const char delim)
{
    int i = *position;
    int k = 0;
    char* output = malloc(256*sizeof(char));

    while (!(buffer[i] == delim || buffer[i] == '\n')\
            && k < 255)
    {
        output[k] = buffer[i];
        i++;
        k++;
    }
    
    output[k] = '\0';
    *position = i;
    
    return output;
}

char* make_quote_str(char *buffer, int *position)
{
    int i = *position;
    i += 1;
    int k = 0;
    char esc_char = '\\';
    char *output = malloc(256*sizeof(char));

    while (k < 255 && buffer[i] != '"'\
            && buffer[i] != '\n')
    {
        if (buffer[i] == esc_char) {
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
    
    output[k] = '\0';
    *position = i;
    return output;
}


void append(TOKEN_H array, int index, Token value) {
    array[index] = value;
}


TOKEN_H tokenizer(char *buffer, char delim)
{
    int capacity = 10;
    TOKEN_H tokens = malloc(capacity*sizeof(Token));
    
    int i = 0;
    int pos_char = 0;
    while(buffer[pos_char] != '\0') 
    {
        if (i >= capacity) {
            capacity *= 2;
            tokens = realloc(tokens,\
                    capacity*sizeof(Token));
        }

        if (buffer[pos_char] == delim ||\
            buffer[pos_char] == '\n') {
            pos_char++;
            continue;  
        } else if (buffer[pos_char] == '"') {
            Token token;
            token.type = QUOTE_STR;
            token.value = make_quote_str(buffer, &pos_char);
            append(tokens, i, token);
            i++;
        } else {
            Token token;
            token.type = STR;
            token.value = make_str(buffer, &pos_char, delim);
            append(tokens, i, token);
            i++;
        }
    }

    return tokens;
}


