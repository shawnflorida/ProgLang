#include "include/lexer.h"
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdio.h>
#include "include/token.h"

lexer_t *init_lexer(char *contents)
{

    // allocating memory for lexer struct into lexer memory
    lexer_t *lexer = calloc(1, sizeof(struct LEXER_STRUCT));
    // -> to access contents in structs; in this case, lexer can access contents, i , c in struct
    lexer->contents = contents; // contents is file contents
    // current index
    lexer->i = 0;
    // current character
    lexer->c = contents[lexer->i];

    printf("this is contents: %s\n", contents);

    return lexer;
}

void lexer_advance(lexer_t *lexer)
{

    // if the current character is equal to null space and less than the len of contents,
    // add 1 to index and character should be equal to lexer's content indexed by the current lexer
    if (lexer->c != '\0' && lexer->i < strlen(lexer->contents))
    {
        lexer->i += 1;
        lexer->c = lexer->contents[lexer->i];
    }
};

void lexer_skip_whitespace(lexer_t *lexer)
{
    // 10 is code for new line; meaning while lexer is a space or a new line:
    while (lexer->c == ' ' || lexer->c == 10)
    {
        // advance the lexer
        lexer_advance(lexer);
    }
};

token_t *lexer_get_next_token(lexer_t *lexer)
{

    // while lexer is not a null and i is less than the lexer contents; meaning
    // there are tokens that are needed to be parsed.
    while (lexer->c != '\0' && lexer->i < strlen(lexer->contents))
    {
        if (lexer->c == ' ' || lexer->c == 10)
        {
            lexer_skip_whitespace(lexer);
        }
        if (isalnum(lexer->c))
        {
            return lexer_collect_id(lexer);
        };

        // parsing a string
        if (lexer->c == '"')
        {
            return lexer_collect_string(lexer);
        }
        switch (lexer->c)
        {
        // if the case is =, call lexer advance while carrying the token,
        // add its parameter the current lexer instance, and the result
        // of init_token if we inputted TOKEN_EQUALS, and the current lexer character as a string.
        case '=':
            return lexer_advance_with_token(lexer, init_token(TOKEN_EQUALS, lexer_get_current_char_as_string(lexer)));
            break;
        case ';':
            return lexer_advance_with_token(lexer, init_token(TOKEN_SEMI, lexer_get_current_char_as_string(lexer)));
            break;
        case '(':
            return lexer_advance_with_token(lexer, init_token(TOKEN_LPAREN, lexer_get_current_char_as_string(lexer)));
            break;
        case ')':
            return lexer_advance_with_token(lexer, init_token(TOKEN_RPAREN, lexer_get_current_char_as_string(lexer)));
            break;
        }
    }
    return (void *)0;
};

token_t *lexer_collect_string(lexer_t *lexer)
{
    // if the lexer collected a string, advance it to skip the "
    lexer_advance(lexer);

    // value is the allocation of the memory for a string
    char *value = calloc(1, sizeof(char));
    value[0] = '\0';
    while (lexer->c != '"')
    {
        char *s = lexer_get_current_char_as_string(lexer);
        // reallocate the string length of the value by adding the length of s to update it and fit the string.
        value = realloc(value, (strlen(value) + strlen(s) + 1) * sizeof(char));
        strcat(value, s); // append the current character to value string.
        lexer_advance(lexer);
    }
    lexer_advance(lexer);
    // return the value by calling the init_token function wherein it will be a TOKEN_STRING as type and added into the struct.
    return init_token(TOKEN_STRING, value);
};

token_t *lexer_collect_id(lexer_t *lexer)
{
    // value is the allocation of the memory for a string
    char *value = calloc(1, sizeof(char));
    value[0] = '\0';
    // while the character is an alphanumeric
    while (isalnum(lexer->c))
    {
        char *s = lexer_get_current_char_as_string(lexer);
        // reallocate the string length of the value by adding the length of s to update it and fit the string.
        value = realloc(value, (strlen(value) + strlen(s) + 1) * sizeof(char));
        strcat(value, s); // append the current character to value string.
        lexer_advance(lexer);
    }
    // advance the token.
    // return the value by calling the init_token function wherein it will be a TOKEN_STRING as type and added into the struct.
    return init_token(TOKEN_ID, value);
}

token_t *lexer_advance_with_token(lexer_t *lexer, token_t *token)
{
    lexer_advance(lexer);
    return token;
}

char *lexer_get_current_char_as_string(lexer_t *lexer)
{
    char *str = calloc(2, sizeof(char));
    str[0] = lexer->c;
    str[1] = '\0';

    return str;
};
