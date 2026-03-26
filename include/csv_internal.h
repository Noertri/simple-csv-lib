typedef enum __token_type {
    DELIM = 0,
    STR,
    QUOTE_STR
} TokenType;


typedef struct __token {
    TokenType type;
    char *value;
} Token;


typedef Token* TOKEN_H;

typedef struct __tokens {
    int len;
    TOKEN_H data;
} TOKENS;


TOKENS tokenizer(char *buffer, char delim);


