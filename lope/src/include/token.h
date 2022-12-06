#ifndef TOKEN_H
#define TOKEN_H

typedef struct TOKEN_STRUCT
{
    enum
    {
        /* id is identifiers */
        TOKEN_ID,       //[a-zA-Z0-9]
        TOKEN_EQUALS,   // =
        TOKEN_STRING,   // "[a-!]"
        TOKEN_SEMI,     // ;
        TOKEN_LPAREN,   // (
        TOKEN_RPAREN,   // )
        TOKEN_LBRACKET, // [
        TOKEN_RBRACKET, // ]
        TOKEN_LBRACE,   // {
        TOKEN_RBRACE,   // }
        TOKEN_COMMENT,  // #
        /* ADD: other operators */

        TOKEN_ADD,     // +
        TOKEN_SUB,     // -
        TOKEN_DIV,     // /
        TOKEN_MULT,    // \*
        TOKEN_MOD,     // %
        TOKEN_EXP,     // ^
        TOKEN_LESS,    // <
        TOKEN_GREATER, // >
        TOKEN_NEGATE,  // !
        /* ADD: keywords */
        TOKEN_WHLE,     // habang
        TOKEN_IF,       // kung
        TOKEN_ELSE,     // sakali
        TOKEN_ELIF,     // kundi
        TOKEN_FOR,      // para
        TOKEN_FUNCTION, // depinisyon/dep
        TOKEN_IMPORT,   // angkat
        TOKEN_TRY,      // subukan
        TOKEN_EXCEPT,   // maliban
        TOKEN_FINALLY,  // wakas
        TOKEN_BREAK,    // tigil
        TOKEN_CONTINUE, // tuloy
        TOKEN_RETURN,   // balik
        /* unknown token */
        TOKEN_UNKNOWN // invalid
    } type;

    char *value;

} token_t;

token_t *init_token(int type, char *value);

#endif