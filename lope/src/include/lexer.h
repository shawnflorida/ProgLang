#ifndef LEXER_H
#define LEXER_H

#include "token.h"

typedef struct LEXER_STRUCT
{
    char c;
    unsigned int i;
    char *contents;

} lexer_t;

// this are methods. It takes in a variable wherein it should be an instance of lexer_t
lexer_t *init_lexer(char *contents);

// move the pointer into the next character
void lexer_advance(lexer_t *lexer);

// for whitespace skipping
void lexer_skip_whitespace(lexer_t *lexer);

// call this function to get to the next character
token_t *lexer_get_next_token(lexer_t *lexer);

// defining how to parse the string
token_t *lexer_collect_string(lexer_t *lexer);

token_t *lexer_collect_id(lexer_t *lexer);

// helper method to call lexer advance lexer and return the token.
//  takes in instance of token and lexer
token_t *lexer_advance_with_token(lexer_t *lexer, token_t *token);

char *lexer_get_current_char_as_string(lexer_t *lexer);
int compare_to_keyword(char *identfier, char *keyword);
token_t *lexer_collect_keyword(lexer_t *lexer);

#endif
