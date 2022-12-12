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
    return lexer;
}

void lexer_advance(lexer_t *lexer)
{

    // if the current character is equal to null space and less than the len of contents,
    // add 1 to index and character should be equal to lexer's content indexed by the current lexer
    if ((lexer->c != '\0' || lexer->c != ' ' || lexer->c != '\t') && lexer->i < strlen(lexer->contents))
    {
        lexer->i += 1;
        lexer->c = lexer->contents[lexer->i];
    }
};

void lexer_skip_whitespace(lexer_t *lexer)
{
    // 10 is code for new line; meaning while lexer is a space or a new line:
    while (lexer->c == ' ' || lexer->c == '\n' || lexer->c == '\t' || lexer->c == '\r')
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
        // for whitespace
        if (lexer->c == ' ' || lexer->c == '\t' || lexer->c == '\n' || lexer->c == '\r' || lexer->c == EOF)  
        {
            lexer_skip_whitespace(lexer);
        }

        switch (lexer->c)
        {
        // if the case is =, call lexer advance while carrying the token,
        // add its parameter the current lexer instance, and the result
        // of init_token if we inputted TOKEN_EQUALS, and the current lexer character as a string.
        case '#':
            if (lexer->contents[lexer->i + 1] == '*')
            {
                return lexer_collect_comment_multi(lexer);
                /* return lexer_advance_with_token(lexer, init_token(TOKEN_COMMENT_VALUE_MULTI, lexer_get_operator_ext_as_string(lexer))); */
            } else 
            {
                return lexer_collect_comment_single(lexer);
                /* return lexer_advance_with_token(lexer, init_token(TOKEN_COMMENT_VALUE_SINGLE, lexer_get_current_char_as_string(lexer))); */
            }
        case '=':
            if (lexer->contents[lexer->i + 1] == '=') 
            {
                return lexer_advance_with_token(lexer, init_token(TOKEN_EQ_TO, lexer_get_operator_ext_as_string(lexer)));
            } else
            {
                return lexer_advance_with_token(lexer, init_token(TOKEN_EQUALS, lexer_get_current_char_as_string(lexer)));
            }
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
        case '{':
            return lexer_advance_with_token(lexer, init_token(TOKEN_RBRACE, lexer_get_current_char_as_string(lexer)));
            break;
        case '}':
            return lexer_advance_with_token(lexer, init_token(TOKEN_LBRACE, lexer_get_current_char_as_string(lexer)));
            break;
        case '[':
            return lexer_advance_with_token(lexer, init_token(TOKEN_RBRACKET, lexer_get_current_char_as_string(lexer)));
            break;
        case ']':
            return lexer_advance_with_token(lexer, init_token(TOKEN_LBRACKET, lexer_get_current_char_as_string(lexer)));
            break;
        case '<':
            if (lexer->contents[lexer->i + 1] == '=') 
            {
                return lexer_advance_with_token(lexer, init_token(TOKEN_LS_THAN_EQ, lexer_get_operator_ext_as_string(lexer)));
            } else
            {
                return lexer_advance_with_token(lexer, init_token(TOKEN_LESS, lexer_get_current_char_as_string(lexer)));
            }
            break;
        case '>':
            if (lexer->contents[lexer->i + 1] == '=') 
            {
                return lexer_advance_with_token(lexer, init_token(TOKEN_GR_THAN_EQ, lexer_get_operator_ext_as_string(lexer)));
            } else
            {
                return lexer_advance_with_token(lexer, init_token(TOKEN_GREATER, lexer_get_current_char_as_string(lexer)));
            }
            break;
        case '+':
            if (lexer->contents[lexer->i + 1] == '=') 
            {
                return lexer_advance_with_token(lexer, init_token(TOKEN_ADD_ASGN, lexer_get_operator_ext_as_string(lexer)));
            } else if (lexer->contents[lexer->i + 1] == '+') {
                return lexer_advance_with_token(lexer, init_token(TOKEN_INCR, lexer_get_operator_ext_as_string(lexer)));
            } else
            {
                return lexer_advance_with_token(lexer, init_token(TOKEN_ADD, lexer_get_current_char_as_string(lexer)));
            }
            break;
        case '-':
            if (lexer->contents[lexer->i + 1] == '=') 
            {
                return lexer_advance_with_token(lexer, init_token(TOKEN_SUB_ASGN, lexer_get_operator_ext_as_string(lexer)));
            } else if (lexer->contents[lexer->i + 1] == '-')
            {
                return lexer_advance_with_token(lexer, init_token(TOKEN_DECR, lexer_get_operator_ext_as_string(lexer)));
            } else
            {
                return lexer_advance_with_token(lexer, init_token(TOKEN_SUB, lexer_get_current_char_as_string(lexer)));
            }
            break;
        case '/':
            if (lexer->contents[lexer->i + 1] == '=') 
            {
                return lexer_advance_with_token(lexer, init_token(TOKEN_DIV_ASGN, lexer_get_operator_ext_as_string(lexer)));
            } else
            {
                return lexer_advance_with_token(lexer, init_token(TOKEN_DIV, lexer_get_current_char_as_string(lexer)));
            }
            break;
        case '~':
            if (lexer->contents[lexer->i + 1] == '=') 
            {
                return lexer_advance_with_token(lexer, init_token(TOKEN_INTDIV_ASGN, lexer_get_operator_ext_as_string(lexer)));
            } else
            {
                return lexer_advance_with_token(lexer, init_token(TOKEN_INTDIV, lexer_get_current_char_as_string(lexer)));
            }
            break;
        case '*':
            if (lexer->contents[lexer->i + 1] == '=') 
            {
                return lexer_advance_with_token(lexer, init_token(TOKEN_MULT_ASGN, lexer_get_operator_ext_as_string(lexer)));
            } else if (lexer->contents[lexer->i + 1] == '#')
            {
                return lexer_advance_with_token(lexer, init_token(TOKEN_COMMENT_VALUE_MULTI, lexer_get_operator_ext_as_string(lexer)));
            } else
            {
                return lexer_advance_with_token(lexer, init_token(TOKEN_MULT, lexer_get_current_char_as_string(lexer)));
            }
            break;
        case '%':
            if (lexer->contents[lexer->i + 1] == '=') 
            {
                return lexer_advance_with_token(lexer, init_token(TOKEN_MOD_ASGN, lexer_get_operator_ext_as_string(lexer)));
            } else
            {
                return lexer_advance_with_token(lexer, init_token(TOKEN_LESS, lexer_get_current_char_as_string(lexer)));
            }
            break;
        case '^':
            return lexer_advance_with_token(lexer, init_token(TOKEN_EXP, lexer_get_current_char_as_string(lexer)));
            break;
        case '|':
            if (lexer->contents[lexer-> i + 1] == '|')
            {
                return lexer_advance_with_token(lexer, init_token(TOKEN_OR, lexer_get_operator_ext_as_string(lexer)));
            } else
            {
                return lexer_advance_with_token(lexer, init_token(TOKEN_UNKNOWN, lexer_get_current_char_as_string(lexer)));
            }
            break;
        case '&':
            if (lexer->contents[lexer-> i + 1] == '&')
            {
                return lexer_advance_with_token(lexer, init_token(TOKEN_AND, lexer_get_operator_ext_as_string(lexer)));
            } else
            {
                return lexer_advance_with_token(lexer, init_token(TOKEN_UNKNOWN, lexer_get_current_char_as_string(lexer)));
            }
            break;
        case '!':
            if (lexer->contents[lexer->i + 1] == '=') 
            {
                return lexer_advance_with_token(lexer, init_token(TOKEN_NOT_EQ, lexer_get_operator_ext_as_string(lexer)));
            } else
            {
                return lexer_advance_with_token(lexer, init_token(TOKEN_NEGATE, lexer_get_current_char_as_string(lexer)));
            }
            break;
        case ':':
            return lexer_advance_with_token(lexer, init_token(TOKEN_COLON, lexer_get_current_char_as_string(lexer)));
            break;
        case '_':
            return lexer_advance_with_token(lexer, init_token(TOKEN_UNDERSCORE, lexer_get_current_char_as_string(lexer)));
            break;
        case ',':
            return lexer_advance_with_token(lexer, init_token(TOKEN_COMMA, lexer_get_current_char_as_string(lexer)));
        default:
            // for KEYWORDS
            if (isalpha(lexer->c))
            {
                return lexer_collect_keyword(lexer);
            };

            //for numbers
            if (isdigit(lexer->c))
            {
                    return lexer_collect_number(lexer);
            };

            // FOR THE STRINGS
            if (lexer->c == '"')
            {
                return lexer_collect_string(lexer);
            }

            if (lexer->c == '\'')
            {
                return lexer_collect_char_lit(lexer);
            }

            // Unknown
            return lexer_advance_with_token(lexer, init_token(TOKEN_UNKNOWN, lexer_get_current_char_as_string(lexer)));
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

char* lexer_get_operator_ext_as_string(lexer_t *lexer)
{
    char* value = calloc(3, sizeof(char));
    value[0] = lexer->c;
    lexer_advance(lexer);
    value[1] = lexer->c;
    value[2] = '\0';
    
    return value;
}

token_t *lexer_collect_id(lexer_t *lexer)
{
    // value is the allocation of the memory for a string
    char *value = calloc(1, sizeof(char));
    value[0] = '\0';
    // while the character is an alphanumeric
    while (isalpha(lexer->c))
    {
        char *s = lexer_get_current_char_as_string(lexer);
        // reallocate the string length of the value by adding the length of s to update it and fit the string.
        value = realloc(value, (strlen(value) + strlen(s) + 1) * sizeof(char));
        strcat(value, s); // append the current character to value string.
        lexer_advance(lexer);
    }


    return init_token(TOKEN_ID, value);
}

token_t *lexer_advance_with_token(lexer_t *lexer, token_t *token)
{
    lexer_advance(lexer);
    return token;
}

token_t *lexer_collect_keyword(lexer_t *lexer)
{
    // value is the allocation of the memory for a string
    char *value = calloc(1, sizeof(char));
    value[0] = '\0';
    // while the character is an alphanumeric
    while ((lexer->c != ' ' && lexer->c != '\n' && lexer->c != '\t') && isalnum(lexer->c) || lexer->c == '_')
    {
        char *s = lexer_get_current_char_as_string(lexer);
        // reallocate the string length of the value by adding the length of s to update it and fit the string.
        value = realloc(value, (strlen(value) + strlen(s) + 1) * sizeof(char));
        strcat(value, s); // append the current character to value string.
        lexer_advance(lexer);
    }
    // advance the token.
    // return the value by calling the init_token function wherein it will be a TOKEN_STRING as type and added into the struct.
    if (compare_to_keyword(value, "kung") == 1)
    {
        return init_token(TOKEN_IF, value);
    }
    else if (compare_to_keyword(value, "habang") == 1)
    {
        return init_token(TOKEN_WHLE, value);
    }
    else if (compare_to_keyword(value, "sakali") == 1)
    {
        return init_token(TOKEN_ELSE, value);
    }
    else if (compare_to_keyword(value, "kundi") == 1)
    {
        return init_token(TOKEN_ELIF, value);
    }
    else if (compare_to_keyword(value, "para") == 1)
    {
        return init_token(TOKEN_FOR, value);
    }
    else if (compare_to_keyword(value, "dep") == 1)
    {
        return init_token(TOKEN_FUNCTION, value);
    }
    else if (compare_to_keyword(value, "depinisyon") == 1)
    {
        return init_token(TOKEN_FUNCTION, value);
    }
    else if (compare_to_keyword(value, "angkat") == 1)
    {
        return init_token(TOKEN_IMPORT, value);
    }
    else if (compare_to_keyword(value, "subukan") == 1)
    {
        return init_token(TOKEN_TRY, value);
    }
    else if (compare_to_keyword(value, "maliban") == 1)
    {
        return init_token(TOKEN_EXCEPT, value);
    }
    else if (compare_to_keyword(value, "wakas") == 1)
    {
        return init_token(TOKEN_FINALLY, value);
    }
    else if (compare_to_keyword(value, "tigil") == 1)
    {
        return init_token(TOKEN_BREAK, value);
    }
    else if (compare_to_keyword(value, "tuloy") == 1)
    {
        return init_token(TOKEN_CONTINUE, value);
    }
    else if (compare_to_keyword(value, "balik") == 1)
    {
        return init_token(TOKEN_RETURN, value);
    }
    else if (compare_to_keyword(value, "at") == 1)
    {
        return init_token(TOKEN_AND, value);
    }
    else if (compare_to_keyword(value, "o") == 1)
    {
        return init_token(TOKEN_OR, value);
    }
    else if (compare_to_keyword(value, "nasa") == 1)
    {
        return init_token(TOKEN_IN, value);
    }
    else if (compare_to_keyword(value, "lahat") == 1)
    {
        return init_token(TOKEN_GLOBAL, value);
    }
    else if (compare_to_keyword(value, "bolyan") == 1)
    {
        return init_token(TOKEN_BOOL, value);
    }
    else if (compare_to_keyword(value, "totoo") == 1)
    {
        return init_token(TOKEN_BOOLT, value);
    }
    else if (compare_to_keyword(value, "mali") == 1)
    {
        return init_token(TOKEN_BOOLM, value);
    }
    else if (compare_to_keyword(value, "numero") == 1)
    {
        return init_token(TOKEN_INT, value);
    }
    else if (compare_to_keyword(value, "num") == 1)
    {
        return init_token(TOKEN_INT, value);
    }
    else if (compare_to_keyword(value, "linya") == 1)
    {
        return init_token(TOKEN_STR, value);
    }
    else if (compare_to_keyword(value, "wala") == 1)
    {
        return init_token(TOKEN_NULL, value);
    }
    else if (compare_to_keyword(value, "punto") == 1)
    {
        return init_token(TOKEN_FLOAT, value);
    }
    else if (compare_to_keyword(value, "puntonumero") == 1)
    {
        return init_token(TOKEN_FLOAT, value);
    }
    else if (compare_to_keyword(value, "doble") == 1)
    {
        return init_token(TOKEN_DBL, value);
    }
    else if (compare_to_keyword(value, "doblenumero") == 1)
    {
        return init_token(TOKEN_DBL, value);
    }
    else if (compare_to_keyword(value, "karakter") == 1)
    {
        return init_token(TOKEN_CHAR, value);
    }
    else if (compare_to_keyword(value, "kar") == 1)
    {
        return init_token(TOKEN_CHAR, value);
    }
    else
    {
        // count the number of capital in the string
        int counter_caps = 0;
        int flag_illegal_symbol = 0;

        for(int i=0; i<strlen(value); i++)
        {
            if(isupper(value[i]))
                counter_caps++;
            else if ((isalnum(value[i]) == 0 && value[i] != '_') || isdigit(value[i]) == 1)
                flag_illegal_symbol = 1;
        }
        if(counter_caps == strlen(value))
        {
            return init_token(TOKEN_CAPITAL, value);
        }
        if (flag_illegal_symbol == 0)   
            return init_token(TOKEN_ID, value);
        else
            return init_token(TOKEN_UNKNOWN, value);
    }
}

char *lexer_get_current_char_as_string(lexer_t *lexer)
{
    char *str = calloc(2, sizeof(char));
    str[0] = lexer->c;
    str[1] = '\0';

    return str;
};

// temporary way to compare
int compare_to_keyword(char *identifier, char *keyword)
{
    int index;
    // turns the keyword into a finite state machine, checks character by character
    for (index = 0; index < strlen(keyword); index++)
    {
        if (identifier[index] != keyword[index])
        {
            return 0;
        }
    }
    return 1;
}

token_t* lexer_collect_number(lexer_t *lexer) {
    // value is the allocation of the memory for a string
    char *value = calloc(1, sizeof(char));
    value[0] = '\0';
    // while the character is an alphanumeric
    int decimal_count = 0;
    while (isdigit(lexer->c) || lexer->c == '.')
    {
        if (lexer->c)
        {
            decimal_count++;
        }
        char *s = lexer_get_current_char_as_string(lexer);
        // reallocate the string length of the value by adding the length of s to update it and fit the string.
        value = realloc(value, (strlen(value) + strlen(s) + 1) * sizeof(char));
        strcat(value, s); // append the current character to value string.
        lexer_advance(lexer);
    }
    // advance the token.
    // return the value by calling the init_token function wherein it will be a TOKEN_STRING as type and added into the struct.
    
    if (decimal_count < 2 && strlen(value) )
    {
        return init_token(TOKEN_NUM, value);
    } else
    {
        return init_token(TOKEN_UNKNOWN, value);
    }
}

token_t *lexer_collect_comment_single(lexer_t *lexer)
{
    lexer_advance(lexer);
    char *value = calloc(1, sizeof(char));
    value[0] = '\0';
    while (lexer->c != '\n' && lexer->c != '\0')
    {

        char *s = lexer_get_current_char_as_string(lexer);
        value = realloc(value, (strlen(value) + strlen(s) + 1) * sizeof(char));
        strcat(value, s); // append the current character to value string.
        lexer_advance(lexer);
    }
    return init_token(TOKEN_COMMENT_VALUE_SINGLE, value);
};

token_t *lexer_collect_comment_multi(lexer_t *lexer)
{
    lexer_advance(lexer);
    lexer_advance(lexer);
    char *value = calloc(1, sizeof(char));
    value[0] = '\0';
    while (!(lexer->c == '*' && lexer->contents[lexer->i + 1] == '#'))
    {
        char *s = lexer_get_current_char_as_string(lexer);
        value = realloc(value, (strlen(value) + strlen(s) + 1) * sizeof(char));
        strcat(value, s);
        lexer_advance(lexer);
    }
    lexer_advance(lexer); //for *
    lexer_advance(lexer); //for #
    return init_token(TOKEN_COMMENT_VALUE_MULTI, value);
};

token_t *lexer_collect_char_lit(lexer_t *lexer)
{
    lexer_advance(lexer);
    char *value = calloc(2, sizeof(char));
    value[0] = lexer->c;
    value[1] = '\0';
    if (lexer->contents[lexer->i + 1] == 39)
    {
        lexer_advance(lexer);
        lexer_advance(lexer);
        return init_token(TOKEN_CHAR_LIT, value);
    } else
    {
        lexer_advance(lexer);
        lexer_advance(lexer);
        return init_token(TOKEN_UNKNOWN, value);
    }
};