typedef enum __token_type {
    DELIM = 0,
    FIELD,
    EOL
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
    char *null_value;
} CSVOpts;
#endif


TOKENS tokenizer(char *buffer, const CSVOpts options);


