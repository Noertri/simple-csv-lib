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

#ifndef OPTIONS
#define OPTIONS
typedef struct __options {
    char delimiter;
    char quote;
    char escape;
} CSV_OPTS;
#endif


TOKENS tokenizer(char *buffer, const CSV_OPTS options);


