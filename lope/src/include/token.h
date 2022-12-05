#ifndef TOKEN_H
#define TOKEN_H

typedef struct TOKEN_STRUCT
{
    enum
    {
        // id is identifiers
        TOKEN_ID,
        TOKEN_EQUALS,
        TOKEN_STRING,
        TOKEN_SEMI,
        TOKEN_LPAREN,
        TOKEN_RPAREN,
        // ADD: other operators
        TOKEN_ADD,
        TOKEN_SUB,
        TOKEN_DIV,
        TOKEN_MULT,
        TOKEN_MOD,
        TOKEN_EXP,
    } type;

    char *value;

} token_t;

token_t *init_token(int type, char *value);

#endif