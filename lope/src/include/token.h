#ifndef TOKEN_H
#define TOKEN_H

typedef enum {
    /* id is identifiers */
    TOKEN_ID,        // [a-zA-Z]*
    TOKEN_EQUALS,    // =
    TOKEN_SEMI,      // ;
    TOKEN_LPAREN,    // (
    TOKEN_RPAREN,    // )
    TOKEN_LBRACKET,  // [
    TOKEN_RBRACKET,  // ]
    TOKEN_LBRACE,    // {
    TOKEN_RBRACE,    // }
    TOKEN_COMMENT,   // #

    /* ADD: operators */
    TOKEN_ADD,         // +
    TOKEN_SUB,         // -
    TOKEN_DIV,         // /
    TOKEN_INTDIV,      // ~
    TOKEN_MULT,        // **
    TOKEN_MOD,         // %
    TOKEN_EXP,         // ^
    TOKEN_LESS,        // <
    TOKEN_GREATER,     // >
    TOKEN_NEGATE,      // !
    TOKEN_COLON,       // :
    TOKEN_UNDERSCORE,  // _

    // Unary
    TOKEN_INCR,  // ++
    TOKEN_DECR,  // --

    // Relational
    TOKEN_EQ_TO,       // ==
    TOKEN_LS_THAN_EQ,  // <=
    TOKEN_GR_THAN_EQ,  // >=
    TOKEN_NOT_EQ,      // !=

    // Operator + Assignment
    TOKEN_ADD_ASGN,     // +=
    TOKEN_SUB_ASGN,     // -=
    TOKEN_MULT_ASGN,    // **=
    TOKEN_DIV_ASGN,     // /=
    TOKEN_MOD_ASGN,     // %=
    TOKEN_INTDIV_ASGN,  // ~=

    /* ADD: keywords */
    TOKEN_HABANG,   // habang
    TOKEN_KUNG,     // kung
    TOKEN_SAKALI,   // sakali
    TOKEN_KUNDI,    // kundi
    TOKEN_PARA,     // para
    TOKEN_DEP,      // depinisyon/dep
    TOKEN_ANGKAT,   // angkat
    TOKEN_SUBUKAN,  // subukan
    TOKEN_MALIBAN,  // maliban
    TOKEN_WAKAS,    // wakas
    TOKEN_TIGIL,    // tigil
    TOKEN_TULOY,    // tuloy
    TOKEN_BALIK,    // balik
    TOKEN_AT,       // at
    TOKEN_O,        // o
    TOKEN_NASA,     // nasa
    TOKEN_LAHAT,    // lahat

    /* ADD: data types*/
    TOKEN_BOLYAN,  // bolyan
    TOKEN_TOTOO,   // totoo
    TOKEN_MALI,    // mali
    TOKEN_NUMERO,  // numero / num
    TOKEN_NUM,     // num literals [0-9]*
    TOKEN_LINYA,   // linya
    TOKEN_STRING,  // "[a-!]"
    TOKEN_WALA,    // wala
    TOKEN_PUNTO,   // punto / puntonumero
    TOKEN_DOBLE,   // doble / doblenumero
    TOKEN_KAR,     // karakter / kar

    /* unknown token */
    TOKEN_UNKNOWN,  // invalid
    // for comments
    TOKEN_COMMENT_VALUE_SINGLE,  // #
    TOKEN_COMMENT_VALUE_MULTI,   // #*
    // for constant
    TOKEN_CAPITAL,    // [A-Z]*
    TOKEN_COMMA,      // ,
    TOKEN_CHAR_LIT,   // "[a-z]"
    TOKEN_FLOAT_LIT,  // num.num | num
    TOKEN_LAHAD,      // lahad
    TOKEN_KUHA,       // kuha
    TOKEN_IBURA,      // ibura
    TOKEN_FROM,       // angkat
    TOKEN_ADDRESS
} type;
typedef struct TOKEN_STRUCT {
    type type;
    char *value;
    unsigned int lpos;
    unsigned int cpos;

} token_t;

token_t *init_token(int type, char *value, int l, int lc);

#endif