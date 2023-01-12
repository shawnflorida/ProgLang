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
    //inputting of file contents into lexer struct
    lexer->contents = contents; // contents is file contents
    // current index
    //assignments of index into lexer struct i
    lexer->i = 0;
    // current character
    lexer->c = contents[lexer->i];
    return lexer;
}

//lexer-advance -> iurong natin yung current index ng character sa loob ng contents
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

//for skipping whitespace
void lexer_skip_whitespace(lexer_t *lexer)
{
    // 10 is code for new line; meaning while lexer is a space or a new line:
    while (lexer->c == ' ' || lexer->c == '\n' || lexer->c == '\t' || lexer->c == '\r')
    {
        // advance the lexer
        lexer_advance(lexer);
    }
};

// Special Characters 
// check if current character is a special character and if the next struct index is also a special character
// if so, return the token, and return the special character as a string
token_t *lexer_get_next_token(lexer_t *lexer)
{
    // while lexer is not a null and i is less than the lexer contents; meaning
    // there are tokens that are needed to be parsed.
    while (lexer->c != '\0' && lexer->i < strlen(lexer->contents))
    {
        if (lexer->c == ' ' || lexer->c == '\t' || lexer->c == '\n' || lexer->c == '\r' || lexer->c == '\f')
        {
            lexer_skip_whitespace(lexer);
        }
        // -> for calling value or contents of struct
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
        case '=': //2times
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
            // linya 
            //c = i; 
            if (isalpha(lexer->c))
            {
                //is to append all characters that are not special characters and check if it is a keyword, noise, or reserved word
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
            if (lexer->c == '\0')
            {
                break;
            }
            // Unknown
            return lexer_advance_with_token(lexer, init_token(TOKEN_UNKNOWN, lexer_get_current_char_as_string(lexer)));
            break;
        }
    }
    return (void *)0;
};


// for string literals
// if the current character is " call this function and advance the lexer
token_t *lexer_collect_string(lexer_t *lexer)
{
    // if the lexer collected a string, advance it to skip the "
    lexer_advance(lexer);

    // value is the allocation of the memory for a string
    char *value = calloc(1, sizeof(char));
    value[0] = '\0';
    //if the current character is not a ". then, everything between " ", make into a string. 
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
    while ((lexer->c != ' ' && lexer->c != '\n' && lexer->c != '\t' && lexer->c != '\0') && isalnum(lexer->c) || lexer->c == '_') //isalnum(lexer->c) || lexer->c == '_'
    {
        char *s = lexer_get_current_char_as_string(lexer);
        // reallocate the string length of the value by adding the length of s to update it and fit the string.
        value = realloc(value, (strlen(value) + strlen(s) + 1) * sizeof(char));
        strcat(value, s); // append the current character to value string.
        lexer_advance(lexer);
    }
    // advance the token.
    // return the value by calling the init_token function wherein it will be a TOKEN_STRING as type and added into the struct.
    //TOKEN_WHILE - HABANG
    if (value[0]== 'h') {
        if (value[1] == 'a') {
            if (value[2] == 'b') {
                if (value[3] == 'a') {
                    if (value[4] == 'n') {
                        if (value[5] == 'g') {
                            if (value[6] == '\0') 
                                return init_token(TOKEN_WHLE, value);
                            else 
                                goto SCAN_INDENTIFIER;
                        } else goto SCAN_INDENTIFIER;
                    } else goto SCAN_INDENTIFIER;
                } else goto SCAN_INDENTIFIER;
            } else goto SCAN_INDENTIFIER;
        } else goto SCAN_INDENTIFIER;
    }
    //TOKEN_ELSE - SAKALI | //TOKEN_TRY - SUBUKAN
    else if (value[0]== 's') {
        if (value[1] == 'a') {
            if (value[2] == 'k') {
                if (value[3] == 'a') {
                    if (value[4] == 'l') {
                        if (value[5] == 'i') {
                            if (value[6] == '\0') 
                                return init_token(TOKEN_ELSE, value);
                            else 
                                goto SCAN_INDENTIFIER;
                        } else goto SCAN_INDENTIFIER;
                    } else goto SCAN_INDENTIFIER;
                } else goto SCAN_INDENTIFIER;
            } else goto SCAN_INDENTIFIER;
        } else if (value[1] == 'u') {
            if (value[2] == 'b') {
                if (value[3] == 'u') {
                    if (value[4] == 'k') {
                        if (value[5] == 'a'){
                            if (value[6] == 'n'){
                                if (value[7] == '\0')
                                    return init_token(TOKEN_TRY, value);
                                else 
                                    goto SCAN_INDENTIFIER;
                            } else goto SCAN_INDENTIFIER;
                        } else goto SCAN_INDENTIFIER;
                    } else goto SCAN_INDENTIFIER;
                } else goto SCAN_INDENTIFIER;
            } else goto SCAN_INDENTIFIER;
        } else goto SCAN_INDENTIFIER;
    }
    //TOKEN_FOR - PARA | //TOKEN_FLOAT - PUNTONUMERO/PUNTO
    else if (value[0]== 'p') {
        if (value[1] == 'a') {
            if (value[2] == 'r') {
                if (value[3] == 'a') {
                    if (value[4] == '\0')
                        return init_token(TOKEN_FOR, value);
                    else 
                        goto SCAN_INDENTIFIER;
                } else goto SCAN_INDENTIFIER;
            } else goto SCAN_INDENTIFIER;
        } else if (value[1] == 'u') {
            if (value[2] == 'n') {
                if (value[3] == 't') {
                    if (value[4] == 'o') {
                        if (value[5] == 'n') {
                            if (value[6] == 'u') {
                                if (value[7] == 'm') {
                                    if (value[8] == 'e') {
                                        if (value[9] == 'r') {
                                            if (value[10] == 'o') {
                                                if (value[11] == '\0')
                                                    return init_token(TOKEN_FLOAT, value);
                                                else 
                                                    goto SCAN_INDENTIFIER;
                                            } else goto SCAN_INDENTIFIER;
                                        } else goto SCAN_INDENTIFIER;
                                    } else goto SCAN_INDENTIFIER;
                                } else goto SCAN_INDENTIFIER;
                            } else goto SCAN_INDENTIFIER;
                        } else if (value[5] == '\0') {
                            return init_token(TOKEN_FLOAT, value);
                        } else goto SCAN_INDENTIFIER;
                    } else goto SCAN_INDENTIFIER;
                } else goto SCAN_INDENTIFIER;
            } else goto SCAN_INDENTIFIER;
        } else goto SCAN_INDENTIFIER;
    }
    //TOKEN_GLOBAL - LAHAT |  //TOKEN_STRING - LINYA
    else if(value[0] == 'l') {
        if(value[1] == 'a') {
            if(value[2] == 'h') {
                if(value[3] == 'a') {
                    if(value[4] == 'd') {
                        if(value[5] == '\0') 
                            return init_token(TOKEN_PRINT, value);
                    }
                    else if(value[4] == 't') {
                        if(value[5] == '\0') 
                            return init_token(TOKEN_GLOBAL, value);
                    } else goto SCAN_INDENTIFIER;
                } else goto SCAN_INDENTIFIER;
            } else goto SCAN_INDENTIFIER;
        } else if (value[1] == 'i') {
            if (value[2] == 'n') {
                if (value[3] == 'y') {
                    if (value[4] == 'a') {
                        if(value[5] == '\0') 
                            return init_token(TOKEN_STR, value);
                        else 
                            goto SCAN_INDENTIFIER;
                    } else goto SCAN_INDENTIFIER;
                } else goto SCAN_INDENTIFIER;
            } else goto SCAN_INDENTIFIER;
        } else goto SCAN_INDENTIFIER;
    }
    //TOKEN_BOOLT - TOTOO
    else if(value[0] == 't') {
        if(value[1] == 'o') {
            if(value[2] == 't') {
                if(value[3] == 'o') {
                    if(value[4] == 'o') {
                        if(value[5] == '\0') 
                            return init_token(TOKEN_BOOLT, value);
                        else
                            goto SCAN_INDENTIFIER;
                    } else goto SCAN_INDENTIFIER;
                } else goto SCAN_INDENTIFIER;
            } else goto SCAN_INDENTIFIER;
        } else if (value[1] == 'i') {
            if (value[2] == 'g') {
                if (value[3] == 'i') {
                    if (value[4] == 'l') {
                        if (value[5] == '\0')
                            return init_token(TOKEN_BREAK, value);
                        else
                            goto SCAN_INDENTIFIER;
                    } else goto SCAN_INDENTIFIER;
                } else goto SCAN_INDENTIFIER;
            } else goto SCAN_INDENTIFIER;
        } else if (value[1] == 'u') {
            if (value[2] == 'l') {
                if (value[3] == 'o') {
                    if (value[4] == 'y') {
                        if (value[5] == '\0') 
                            return init_token(TOKEN_CONTINUE, value);
                        else 
                            goto SCAN_INDENTIFIER;
                    } else goto SCAN_INDENTIFIER;
                } else goto SCAN_INDENTIFIER;
            } else goto SCAN_INDENTIFIER;
        } else goto SCAN_INDENTIFIER;
    }
    //TOKEN_INT - NUMERO / NUM | //TOKEN_IN - NASA
    else if(value[0] == 'n') {
        if(value[1] == 'u') {
            if(value[2] == 'm') {
                if(value[3] == 'e') {
                    if(value[4] == 'r') {
                        if(value[5] == 'o') {
                            if(value[6] == '\0') 
                                return init_token(TOKEN_INT, value);
                            else 
                                goto SCAN_INDENTIFIER;
                        } else 
                            goto SCAN_INDENTIFIER;
                    } else 
                        goto SCAN_INDENTIFIER; 
                } else if (value[3] == '\0') {
                    return init_token(TOKEN_INT, value);
                } else 
                    goto SCAN_INDENTIFIER;
            } else 
                goto SCAN_INDENTIFIER;
        } else if (value[1] == 'a') {
            if (value[2] == 's') {
                if (value[3] == 'a') {
                    if (value[4] == '\0') {
                        return init_token(TOKEN_IN, value);
                    } else 
                        goto SCAN_INDENTIFIER;        
                } else 
                    goto SCAN_INDENTIFIER;
            } else 
                goto SCAN_INDENTIFIER;
        } else 
            goto SCAN_INDENTIFIER;
    }
    //TOKEN_CHAR - KARAKTER/KAR | //TOKEN_ELIF - KUNDI | //TOKEN_IF - KUNG | //TOKEN_KUHA - KUHA
    else if (value[0] == 'k') {
        if (value[1] == 'a') {
            if (value[2] == 'r') {
                if (value[3] == 'a') {
                    if (value[4] == 'k') {
                        if (value[5] == 't') {
                            if (value[6] == 'e') {
                                if (value[7] == 'r') {
                                    if (value[8] == '\0') {
                                        return init_token(TOKEN_CHAR, value);
                                    } else 
                                        goto SCAN_INDENTIFIER;
                                } else 
                                    goto SCAN_INDENTIFIER;
                            } else 
                                goto SCAN_INDENTIFIER;
                        } else 
                            goto SCAN_INDENTIFIER;
                    } else 
                        goto SCAN_INDENTIFIER;
                } else if (value[3] == '\0') {
                    return init_token(TOKEN_CHAR, value);
                } else goto SCAN_INDENTIFIER;
            } else goto SCAN_INDENTIFIER;
        } else if (value[1] == 'u') {
            if (value[2] == 'n') {
                if (value[3] == 'd') {
                    if (value[4] == 'i') {
                        if (value[5] == '\0')
                            return init_token(TOKEN_ELIF, value);
                        else 
                            goto SCAN_INDENTIFIER;
                    } else 
                        goto SCAN_INDENTIFIER;
                }
                else if (value[3] == 'g') {
                    if (value[4] == '\0')
                        return init_token(TOKEN_IF, value);
                    else
                        goto SCAN_INDENTIFIER;
                } else goto SCAN_INDENTIFIER; 
            } else if (value[2] == 'h') {
                if (value[3] == 'a') {
                    if (value[4] == '\0')
                        return init_token(TOKEN_SCAN, value);
                    else 
                        goto SCAN_INDENTIFIER;
                } else goto SCAN_INDENTIFIER;
            } else goto SCAN_INDENTIFIER;
        } else goto SCAN_INDENTIFIER;
    }
    //TOKEN_FUNCTION - DEPINISYON/DEP - //TOKEN_DBL - DOBLENUMERO/DOBLE
    else if (value[0] == 'd') {
        if (value[1] == 'e') {
            if (value[2] == 'p') {
                if (value[3] == 'i') {
                    if (value[4] == 'n'){
                        if (value[5] == 'i'){
                            if (value[6] == 's'){
                                if (value[7] == 'y'){
                                    if (value[8] == 'o'){
                                        if (value[9] == 'n'){
                                            if (value[10] == '\0')
                                                return init_token(TOKEN_FUNCTION, value);
                                            else
                                                goto SCAN_INDENTIFIER;
                                        } else goto SCAN_INDENTIFIER;
                                    } else goto SCAN_INDENTIFIER;
                                } else goto SCAN_INDENTIFIER;
                            } else goto SCAN_INDENTIFIER;
                        } else goto SCAN_INDENTIFIER;
                    } else goto SCAN_INDENTIFIER; 
                } else if(value[3] == '\0'){
                    return init_token(TOKEN_FUNCTION, value);
                } else goto SCAN_INDENTIFIER;
            } else goto SCAN_INDENTIFIER;
        }
        else if (value[1] == 'o') {
            if (value[2] == 'b') {
                if (value[3] == 'l') {
                    if (value[4] == 'e') {
                        if (value[5] == 'n') {
                            if (value[6] == 'u') {
                                if (value[7] == 'm') {
                                    if (value[8] == 'e') {
                                        if (value[9] == 'r') {
                                            if (value[10] == 'o') {
                                                if (value[11] == '\0'){
                                                    return init_token(TOKEN_DBL, value);
                                                } else
                                                    goto SCAN_INDENTIFIER;
                                            } else goto SCAN_INDENTIFIER;
                                        } else goto SCAN_INDENTIFIER;
                                    } else goto SCAN_INDENTIFIER;
                                } else goto SCAN_INDENTIFIER;
                            } else goto SCAN_INDENTIFIER;
                        } else if (value[5] == '\0') 
                            return init_token(TOKEN_DBL, value);
                        else 
                            goto SCAN_INDENTIFIER;
                    } else goto SCAN_INDENTIFIER;
                } else goto SCAN_INDENTIFIER;
            } else goto SCAN_INDENTIFIER;
        } else goto SCAN_INDENTIFIER;
    }
    //TOKEN_IMPORT - ANGKAT | //TOKEN_AND - AT
    else if (value[0]== 'a') {
        if (value[1] == 'n') {
            if (value[2] == 'g') {
                if (value[3] == 'k') {
                    if (value[4] == 'a'){
                        if (value[5] == 't'){
                            if (value[6] == '\0')
                                return init_token(TOKEN_IMPORT, value);
                            else
                                goto SCAN_INDENTIFIER;
                        } else goto SCAN_INDENTIFIER;
                    } else goto SCAN_INDENTIFIER;
                } else goto SCAN_INDENTIFIER;
            } else goto SCAN_INDENTIFIER;
        } else if (value[1] == 't') {
            if (value[2] == '\0')
                return init_token(TOKEN_AND, value);
            else
                goto SCAN_INDENTIFIER;
        } else goto SCAN_INDENTIFIER;
    }
    //TOKEN_EXCEPT - MALIBAN | //TOKEN_BOOLM - MALI | //TOKEN_MULA - MULA
    else if (value[0] == 'm') {
        if (value[1] == 'a') {
            if (value[2] == 'l') {
                if (value[3] == 'i') {
                    if (value[4] == 'b'){
                        if (value[5] == 'a'){
                            if (value[6] == 'n'){
                                if (value[7] == '\0'){
                                    return init_token(TOKEN_EXCEPT, value);
                                } else goto SCAN_INDENTIFIER;
                            } else goto SCAN_INDENTIFIER;
                        } else goto SCAN_INDENTIFIER;
                    } else if (value[4] == '\0') {
                        return init_token(TOKEN_BOOLM, value);
                    } else 
                        goto SCAN_INDENTIFIER;
                } else goto SCAN_INDENTIFIER;
            } else goto SCAN_INDENTIFIER; 
        } else if (value[1] == 'u') {
            if (value[2] == 'l') {
                if (value[3] == 'a') {
                    if (value[4] == '\0') {
                        return init_token(TOKEN_FROM, value);
                    } else
                        goto SCAN_INDENTIFIER;
                } else goto SCAN_INDENTIFIER; 
            } else goto SCAN_INDENTIFIER;
        } else goto SCAN_INDENTIFIER;
    }
    //TOKEN_FINALLY - WAKAS | //TOKEN_NULL - WALA
    else if (value[0] == 'w') {
        if (value[1] == 'a') {
            if (value[2] == 'k') {
                if (value[3] == 'a') {
                    if (value[4] == 's'){
                        if (value[5] == '\0'){
                            return init_token(TOKEN_FINALLY, value);
                        } else
                            goto SCAN_INDENTIFIER;
                    } else goto SCAN_INDENTIFIER;
                } else goto SCAN_INDENTIFIER;
            } else if (value[2] == 'l') {
                if (value[3] == 'a') {
                    if (value[4] == '\0')
                        return init_token(TOKEN_NULL, value);
                    else 
                        goto SCAN_INDENTIFIER;
                } else goto SCAN_INDENTIFIER;
            } else goto SCAN_INDENTIFIER;
        } else goto SCAN_INDENTIFIER;
    }
    //TOKEN_LAHAD - LAHAD
    else if (value[0]== 'l') {
        if (value[1] == 'a') {
            if (value[2] == 'h') {
                if (value[3] == 'a') {
                    if (value[4] == 'd') {
                        if (value[5] == '\0')
                            return init_token(TOKEN_PRINT, value);
                        else
                            goto SCAN_INDENTIFIER;
                    } else goto SCAN_INDENTIFIER;
                } else goto SCAN_INDENTIFIER;
            } else goto SCAN_INDENTIFIER;
        } else goto SCAN_INDENTIFIER;
    }
    //TOKEN_BREAK - TIGIL | //TOKEN_CONTINUE - TULOY
    else if (value[0]== 't') {
        if (value[1] == 'i') {
            if (value[2] == 'g') {
                if (value[3] == 'i') {
                    if (value[4] == 'l') {
                        if (value[5] == '\0') {
                            return init_token(TOKEN_BREAK, value);
                        } else
                            goto SCAN_INDENTIFIER;
                    } else goto SCAN_INDENTIFIER;
                } else goto SCAN_INDENTIFIER;
            } else goto SCAN_INDENTIFIER;
        } else if (value[1] == 'u') {
            if (value[2] == 'l') {
                if (value[3] == 'o') {
                    if (value[4] == 'y') {
                        if (value[5] == '\0') {
                            return init_token(TOKEN_CONTINUE, value);
                        } else 
                            goto SCAN_INDENTIFIER;
                    } else goto SCAN_INDENTIFIER;
                } else goto SCAN_INDENTIFIER;
            } else goto SCAN_INDENTIFIER;
        } else goto SCAN_INDENTIFIER;
    }
    //TOKEN_RETURN - BALIK | //TOKEN_BOOL - BOLYAN
    else if (value[0]== 'b') {
        if (value[1] == 'a') {
            if (value[2] == 'l') {
                if (value[3] == 'i') {
                    if (value[4] == 'k') {
                        if (value[5] == '\0')
                            return init_token(TOKEN_RETURN, value);
                        else 
                            goto SCAN_INDENTIFIER;
                    } else goto SCAN_INDENTIFIER;
                } else goto SCAN_INDENTIFIER;
            } else goto SCAN_INDENTIFIER;
        } else if(value[1] == 'o') {
            if(value[2] == 'l') {
                if(value[3] == 'y') {	
                    if(value[4] == 'a') {
                        if(value[5] == 'n') {
                            if(value[6] == '\0') 
                                return init_token(TOKEN_BOOL, value);
                            else
                                goto SCAN_INDENTIFIER;
                        } else goto SCAN_INDENTIFIER;
                    } else goto SCAN_INDENTIFIER;
                } else goto SCAN_INDENTIFIER;
            } else goto SCAN_INDENTIFIER;
        } else goto SCAN_INDENTIFIER;
    }
    //TOKEN_OR - O
    else if (value[0]== 'o'){
        if (value[1] == '\0') 
            return init_token(TOKEN_OR, value);
        else 
            goto SCAN_INDENTIFIER;
    }
    //TOKEN_IBURA - IBURA
    else if (value[0]== 'i') {
        if (value[1] == 'b') {
            if (value[2] == 'u') {
                if (value[3] == 'r') {
                    if (value[4] == 'a') {
                        if (value[5] == '\0') 
                            return init_token(TOKEN_DEL, value);
                        else 
                            goto SCAN_INDENTIFIER;
                    } else goto SCAN_INDENTIFIER;
                } else goto SCAN_INDENTIFIER;
            } else goto SCAN_INDENTIFIER;
        } else goto SCAN_INDENTIFIER;
    }
    else
    {
        SCAN_INDENTIFIER:;
        // john  -> return as id
        // j&&hn -> return as unknown or invalid
        // JOHN = 6 -> return as CONSTNT
        // count the number of capital in the string
        int counter_caps = 0;
        int flag_illegal_symbol = 0;
        int counter_underscore = 0;

        for(int i=0; i<strlen(value); i++)
        {
            if(value[i] == '_')
                counter_underscore++;
            if(isupper(value[i]))
                counter_caps++;
            else if ((isalnum(value[i]) == 0 && value[i] != '_') || isdigit(value[i]) == 1)
                flag_illegal_symbol = 1;
        }
        if((counter_caps + counter_underscore) == strlen(value))
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

//converts digit into string
token_t* lexer_collect_number(lexer_t *lexer) {
    // value is the allocation of the memory for a string
    char *value = calloc(1, sizeof(char));
    value[0] = '\0';
    // while the character is an alphanumeric
    int decimal_count = 0;
    int flag_invalid = 0;
    while ((isdigit(lexer->c) == 1 || lexer->c == '.'))
    {
        if (lexer->c == '.')
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
    if (decimal_count == 0)
    {
        return init_token(TOKEN_NUM, value);
    } else if (decimal_count <= 1)
    {
        return init_token(TOKEN_FLOAT_LIT, value);
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
    while (!(lexer->c == '*' && lexer->contents[lexer->i + 1] == '#') && lexer->c != '\0')
    {
        char *s = lexer_get_current_char_as_string(lexer);
        value = realloc(value, (strlen(value) + strlen(s) + 1) * sizeof(char));
        strcat(value, s);
        lexer_advance(lexer);
    }
    if (lexer->c == '*' && lexer->contents[lexer->i+1] == '#')
    {
        lexer_advance(lexer); //for *
        lexer_advance(lexer); //for #
    } else 
    {
        return init_token(TOKEN_UNKNOWN, value);
    }
    
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
/* 
//TOKEN_WHILE - HABANG
int isHabang(char* lexeme) {
    if (lexeme[0]== 'h') {
        if (lexeme[1] == 'a') {
            if (lexeme[2] == 'b') {
                if (lexeme[3] == 'a') {
                    if (lexeme[4] == 'n') {
                        if (lexeme[5] == 'g') {
                            if (lexeme[6] == '\0') 
                                return 1;
                        }
                    }
                }
            }
        }
    }
    return 0;
}


//TOKEN_IF - KUNG
int isKung(char* lexeme) {
    if (lexeme[0]== 'k') {
        if (lexeme[1] == 'u') {
            if (lexeme[2] == 'n') {
                if (lexeme[3] == 'g') {
                    if (lexeme[4] == '\0')
                        return 1;
                }
            }
        }
    }
    return 0;
}


//TOKEN_ELSE - SAKALI
int isSakali(char* lexeme) {
    if (lexeme[0]== 's') {
        if (lexeme[1] == 'a') {
            if (lexeme[2] == 'k') {
                if (lexeme[3] == 'a') {
                    if (lexeme[4] == 'l') {
                        if (lexeme[5] == 'i') {
                            if (lexeme[6] == '\0') 
                                return 1;
                        }
                    }
                }
            }
        }
    }
    return 0;
}


//TOKEN_ELIF - KUNDI
int collect_kundi(char* lexeme) {
    if (lexeme[0]== 'k') {
        if (lexeme[1] == 'u') {
            if (lexeme[2] == 'n') {
                if (lexeme[3] == 'd') {
                    if (lexeme[4] == 'i') {
                        if (lexeme[5] == '\0')
                            return 1;
                    }
                }
            }
        }
    }
    return 0;
}


//TOKEN_FOR - PARA
int collect_para(char* lexeme) {
    if (lexeme[0]== 'p') {
        if (lexeme[1] == 'a') {
            if (lexeme[2] == 'r') {
                if (lexeme[3] == 'a') {
                    if (lexeme[4] == '\0')
                        return 1;
                }
            }
        }
    }
    return 0;
}

//TOKEN_GLOBAL - LAHAT
int collect_lahat (char* lexeme) {
	if(lexeme[0] == 'l') {
		if(lexeme[1] == 'a') {
			if(lexeme[2] == 'h') {
				if(lexeme[3] == 'a') {
					if(lexeme[4] == 't') {
						if(lexeme[5] == '\0') 
							return 1; 
					}
				}
			}
		}
	}
	return 0;
}

//TOKEN_BOOL - BOLYAN
int collect_bolyan (char* lexeme) {
	if(lexeme[0] == 'b') {
		if(lexeme[1] == 'o') {
			if(lexeme[2] == 'l') {
				if(lexeme[3] == 'y') {	
					if(lexeme[4] == 'a') {
						if(lexeme[5] == 'n') {
							if(lexeme[6] == '\0') 
								return 1; 
						}
					}
				}
			}
		}
	}
	return 0;
}

//TOKEN_BOOLT - TOTOO
int collect_totoo (char* lexeme) {
	if(lexeme[0] == 't') {
		if(lexeme[1] == 'o') {
			if(lexeme[2] == 't') {
				if(lexeme[3] == 'o') {
					if(lexeme[4] == 'o') {
						if(lexeme[5] == '\0') 
							return 1; 
					}
				}
			}
		}
	}
	return 0;
}

//TOKEN_BOOLM - MALI
int collect_mali (char* lexeme) {
	if(lexeme[0] == 'm') {
		if(lexeme[1] == 'a') {
			if(lexeme[2] == 'l') {
				if(lexeme[3] == 'i') {
					if(lexeme[4] == '\0') 
						return 1; 
					
				}
			}
		}
	}
	return 0;
}

//TOKEN_INT - NUMERO / NUM
int collect_numero (char* lexeme) {
	if(lexeme[0] == 'n') {
		if(lexeme[1] == 'u') {
			if(lexeme[2] == 'm') {
				if(lexeme[3] == 'e') {
					if(lexeme[4] == 'r') {
						if(lexeme[5] == 'o') {
							if(lexeme[6] == '\0') 
								return 1; 

						}
					}
				} else if (lexeme[3] == '\0') {
					return 1;
                }
			}
		}
	}
	return 0;
}

//TOKEN_STRING - LINYA
int collect_linya(char* lexeme) {
    if (lexeme[0]== 'l') {
        if (lexeme[1] == 'i') {
            if (lexeme[2] == 'n') {
                if (lexeme[3] == 'y') {
                    if (lexeme[4] == 'a') {
                        if(lexeme[5] == '\0') {
                            return 1;
                        }
                    }
                }
            }
        }
    }
    return 0;
}


//TOKEN_NULL - WALA
int collect_wala(char* lexeme) {
    if (lexeme[0]== 'w') {
            if (lexeme[1] == 'a') {
                if (lexeme[2] == 'l') {
                    if (lexeme[3] == 'a') {
                        if (lexeme[4] == '\0')
                            return 1;
                }
            }
    	}
    }
    return 0;
}


//TOKEN_FLOAT - PUNTONUMERO/PUNTO
int collect_puntonumero(char* lexeme){
    if (lexeme[0]== 'p') {
        if (lexeme[1] == 'u') {
            if (lexeme[2] == 'n') {
                if (lexeme[3] == 't') {
                    if (lexeme[4] == 'o') {
                        if (lexeme[5] == 'n') {
                            if (lexeme[6] == 'u') {
                                if (lexeme[7] == 'm') {
                                    if (lexeme[8] == 'e') {
                                        if (lexeme[9] == 'r') {
                                            if (lexeme[10] == 'o') {
                                                if (lexeme[11] == '\0') {
                                                    return 1;
                                                }
                                            }
                                        }
                                    }
                                }
                            }
                        } else if (lexeme[5] == '\0') {
                            return 1;
                        }
                    }
                }
            }
        }
    }
    return 0;
}


//TOKEN_DBL - DOBLENUMERO/DOBLE
int collect_doblenumero(char* lexeme) {
    if (lexeme[0]== 'd') {
            if (lexeme[1] == 'o') {
                if (lexeme[2] == 'b') {
                    if (lexeme[3] == 'l') {
			if (lexeme[4] == 'e') {
			    if (lexeme[5] == 'n') {
				if (lexeme[6] == 'u') {
				    if (lexeme[7] == 'm') {
					if (lexeme[8] == 'e') {
					    if (lexeme[9] == 'r') {
						if (lexeme[10] == '0') {
						    if (lexeme[11] == '\0')
                          			    return 1;
			     else if (lexeme[5] == '\0')
				return 1;
                			}
           		     	    }
			        }
		            }
   		        }
	            }
	        }
            }
        }
    }
}
    return 0;
}


//TOKEN_CHAR - KARAKTER/KAR
int collect_karakter(char* lexeme) {
    if (lexeme[0]== 'k') {
        if (lexeme[1] == 'a') {
            if (lexeme[2] == 'r') {
                if (lexeme[3] == 'a') {
                    if (lexeme[4] == 'k') {
			            if (lexeme[5] == 't') {
				            if (lexeme[6] == 'e') {
				                if (lexeme[7] == 'r') {
					                if (lexeme[8] == '\0') {
					                    return 1;
                                    }
                  	            }
               	            }
                        } 
	                }
  	            }
		    } else if (lexeme[3] == '\0') {
                            return 1;
            }
        }
    }
    return 0;
}

//TOKEN_FUNCTION - DEPINISYON
int collect_depinisyon(char* lexeme) {
    if (lexeme[0]== 'd') {
        if (lexeme[1] == 'e') {
            if (lexeme[2] == 'p') {
                if (lexeme[3] == 'i') {
                    if (lexeme[4] == 'n'){
                        if (lexeme[5] == 'i'){
                            if (lexeme[6] == 's'){
                                if (lexeme[7] == 'y'){
                                    if (lexeme[8] == 'o'){
                                        if (lexeme[9] == 'n'){
                                            if (lexeme[10] == '\0'){
                                                return 1;
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }
                }else if(lexeme[3] == '\0'){
                    return 1;
                }
            }
        }
    }
    return 0;
}

//TOKEN_IMPORT - ANGKAT
int collect_angkat(char* lexeme) {
    if (lexeme[0]== 'a') {
        if (lexeme[1] == 'n') {
            if (lexeme[2] == 'g') {
                if (lexeme[3] == 'k') {
                    if (lexeme[4] == 'a'){
                        if (lexeme[5] == 't'){
                            if (lexeme[6] == '\0'){
                                return 1;
                            }
                        }
                    }
                }
            }
        }
    }
    return 0;
}

//TOKEN_TRY - SUBUKAN
int collect_subukan(char* lexeme) {
    if (lexeme[0]== 's') {
        if (lexeme[1] == 'u') {
            if (lexeme[2] == 'b') {
                if (lexeme[3] == 'u') {
                    if (lexeme[4] == 'k') {
                        if (lexeme[5] == 'a'){
                            if (lexeme[6] == 'n'){
                                if (lexeme[7] == '\0'){
                                    return 1;
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    return 0;
}

//TOKEN_EXCEPT - MALIBAN
int collect_maliban(char* lexeme) {
    if (lexeme[0]== 'm') {
        if (lexeme[1] == 'a') {
            if (lexeme[2] == 'l') {
                if (lexeme[3] == 'i') {
                    if (lexeme[4] == 'b'){
                        if (lexeme[5] == 'a'){
                            if (lexeme[6] == 'n'){
                                if (lexeme[7] == '\0'){
                                    return 1;
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    return 0;
}

//TOKEN_FINALLY - WAKAS
int collect_wakas(char* lexeme) {
    if (lexeme[0]== 'w') {
        if (lexeme[1] == 'a') {
            if (lexeme[2] == 'k') {
                if (lexeme[3] == 'a') {
                    if (lexeme[4] == 's'){
                        if (lexeme[5] == '\0'){
                            return 1;
                        }
                    }
                }
            }
        }
    }
    return 0;
}

//TOKEN_LAHAD - LAHAD
int collect_lahad(char* lexeme) {
    if (lexeme[0]== 'l') {
            if (lexeme[1] == 'a') {
                if (lexeme[2] == 'h') {
                    if (lexeme[3] == 'a') {
                        if (lexeme[4] == 'd') {
			    if (lexeme[5] == '\0')
                            return 1;
                    }
                }
            }
	}
    }
    return 0;
}

//TOKEN_KUHA - KUHA
int collect_kuha(char* lexeme) {
    if (lexeme[0]== 'k') {
        if (lexeme[1] == 'u') {
            if (lexeme[2] == 'h') {
                if (lexeme[3] == 'a') {
                    if (lexeme[4] == '\0') {
                        return 1;
                    }
                }
            }
	    }
    }
    return 0;
}

//TOKEN_IBURA - IBURA
int collect_lahad(char* lexeme) {
    if (lexeme[0]== 'i') {
        if (lexeme[1] == 'b') {
            if (lexeme[2] == 'u') {
                if (lexeme[3] == 'r') {
                    if (lexeme[4] == 'a') {
                        if (lexeme[5] == '\0') {
                            return 1;
                        }
                    }
                }
            }
	    }
    }
    return 0;
}

//TOKEN_MULA - MULA
int collect_kuha(char* lexeme) {
    if (lexeme[0]== 'm') {
        if (lexeme[1] == 'u') {
            if (lexeme[2] == 'l') {
                if (lexeme[3] == 'a') {
                    if (lexeme[4] == '\0') {
                        return 1;
                    }
                }
            }
	    }
    }
    return 0;
}

//TOKEN_BREAK - TIGIL
int collect_kung(char* lexeme) {
    if (lexeme[0]== 't') {
        if (lexeme[1] == 'i') {
            if (lexeme[2] == 'g') {
                if (lexeme[3] == 'i') {
                    if (lexeme[4] == 'l') {
                        if (lexeme[5] == '\0') {
                            return 1;
                        }
                    }
                }
            }
	    }
    }
    return 0;
}

//TOKEN_CONTINUE - TULOY
int collect_kung(char* lexeme) {
    if (lexeme[0]== 't') {
        if (lexeme[1] == 'u') {
            if (lexeme[2] == 'l') {
                if (lexeme[3] == 'o') {
                    if (lexeme[4] == 'y') {
			            if (lexeme[5] == '\0')
                            return 1;
                    }
                }
            }
	    }
    }
    return 0;
}

//TOKEN_RETURN - BALIK
int collect_kung(char* lexeme) {
    if (lexeme[0]== 'b') {
        if (lexeme[1] == 'a') {
            if (lexeme[2] == 'l') {
                if (lexeme[3] == 'i') {
                    if (lexeme[4] == 'k') {
			            if (lexeme[5] == '\0') {
                            return 1;
                        }
                    }
                }
            }
	    }
    }
    return 0;
}

//TOKEN_AND - AT
int collect_kung(char* lexeme) {
    if (lexeme[0]== 'a') {
        if (lexeme[1] == 't') {
		    if (lexeme[2] == '\0') {
                return 1;
            }
	    }
    }
    return 0;
}

//TOKEN_OR - O
int collect_kung(char* lexeme) {
	if (lexeme[0]== 'o'){
		if (lexeme[1] == '\0') {
			return 1;
        }
	}
	return 0;
}

//TOKEN_IN - NASA
int collect_kung(char* lexeme) {
    if (lexeme[0]== 'n') {
        if (lexeme[1] == 'a') {
            if (lexeme[2] == 's') {
                if (lexeme[3] == 'a') {
			        if (lexeme[4] == '\0') {
                        return 1;
                    }                    
                }
            }
	    }
    }
    return 0;
} */
/* 

 */