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
    // current line
    lexer->l = 1;
    // current char in-line
    lexer->lc = 1;
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
        lexer->lc += 1;
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
        if (lexer->c == '\n') {
            lexer->lc = 0;
            lexer->l += 1;
        }
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
            if (lexer->c == '\n')
                lexer->lc = 0;
            lexer_skip_whitespace(lexer);
        }
        int startPos = lexer->lc;
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
                return lexer_advance_with_token(lexer, init_token(TOKEN_EQ_TO, lexer_get_operator_ext_as_string(lexer), lexer->l, startPos));
            } else
            {
                return lexer_advance_with_token(lexer, init_token(TOKEN_EQUALS, lexer_get_current_char_as_string(lexer), lexer->l, startPos));
            }
            break;
        case ';':
            return lexer_advance_with_token(lexer, init_token(TOKEN_SEMI, lexer_get_current_char_as_string(lexer), lexer->l, startPos));
            break;
        case '(':
            return lexer_advance_with_token(lexer, init_token(TOKEN_LPAREN, lexer_get_current_char_as_string(lexer), lexer->l, startPos));
            break;
        case ')':
            return lexer_advance_with_token(lexer, init_token(TOKEN_RPAREN, lexer_get_current_char_as_string(lexer), lexer->l, startPos));
            break;
        case '{':
            return lexer_advance_with_token(lexer, init_token(TOKEN_RBRACE, lexer_get_current_char_as_string(lexer), lexer->l, startPos));
            break;
        case '}':
            return lexer_advance_with_token(lexer, init_token(TOKEN_LBRACE, lexer_get_current_char_as_string(lexer), lexer->l, startPos));
            break;
        case '[':
            return lexer_advance_with_token(lexer, init_token(TOKEN_RBRACKET, lexer_get_current_char_as_string(lexer), lexer->l, startPos));
            break;
        case ']':
            return lexer_advance_with_token(lexer, init_token(TOKEN_LBRACKET, lexer_get_current_char_as_string(lexer), lexer->l, startPos));
            break;
        case '<':
            if (lexer->contents[lexer->i + 1] == '=') 
            {
                return lexer_advance_with_token(lexer, init_token(TOKEN_LS_THAN_EQ, lexer_get_operator_ext_as_string(lexer), lexer->l, startPos));
            } else
            {
                return lexer_advance_with_token(lexer, init_token(TOKEN_LESS, lexer_get_current_char_as_string(lexer), lexer->l, startPos));
            }
            break;
        case '>':
            if (lexer->contents[lexer->i + 1] == '=') 
            {
                return lexer_advance_with_token(lexer, init_token(TOKEN_GR_THAN_EQ, lexer_get_operator_ext_as_string(lexer), lexer->l, startPos));
            } else
            {
                return lexer_advance_with_token(lexer, init_token(TOKEN_GREATER, lexer_get_current_char_as_string(lexer), lexer->l, startPos));
            }
            break;
        case '+':
            if (lexer->contents[lexer->i + 1] == '=') 
            {
                return lexer_advance_with_token(lexer, init_token(TOKEN_ADD_ASGN, lexer_get_operator_ext_as_string(lexer), lexer->l, startPos));
            } else if (lexer->contents[lexer->i + 1] == '+') {
                return lexer_advance_with_token(lexer, init_token(TOKEN_INCR, lexer_get_operator_ext_as_string(lexer), lexer->l, startPos));
            } else
            {
                return lexer_advance_with_token(lexer, init_token(TOKEN_ADD, lexer_get_current_char_as_string(lexer), lexer->l, startPos));
            }
            break;
        case '-':
            if (lexer->contents[lexer->i + 1] == '=') 
            {
                return lexer_advance_with_token(lexer, init_token(TOKEN_SUB_ASGN, lexer_get_operator_ext_as_string(lexer), lexer->l, startPos));
            } else if (lexer->contents[lexer->i + 1] == '-')
            {
                return lexer_advance_with_token(lexer, init_token(TOKEN_DECR, lexer_get_operator_ext_as_string(lexer), lexer->l, startPos));
            } else
            {
                return lexer_advance_with_token(lexer, init_token(TOKEN_SUB, lexer_get_current_char_as_string(lexer), lexer->l, startPos));
            }
            break;
        case '/':
            if (lexer->contents[lexer->i + 1] == '=') 
            {
                return lexer_advance_with_token(lexer, init_token(TOKEN_DIV_ASGN, lexer_get_operator_ext_as_string(lexer), lexer->l, startPos));
            } else
            {
                return lexer_advance_with_token(lexer, init_token(TOKEN_DIV, lexer_get_current_char_as_string(lexer), lexer->l, startPos));
            }
            break;
        case '~':
            if (lexer->contents[lexer->i + 1] == '=') 
            {
                return lexer_advance_with_token(lexer, init_token(TOKEN_INTDIV_ASGN, lexer_get_operator_ext_as_string(lexer), lexer->l, startPos));
            } else
            {
                return lexer_advance_with_token(lexer, init_token(TOKEN_INTDIV, lexer_get_current_char_as_string(lexer), lexer->l, startPos));
            }
            break;
        case '*':
            if (lexer->contents[lexer->i + 1] == '=') 
            {
                return lexer_advance_with_token(lexer, init_token(TOKEN_MULT_ASGN, lexer_get_operator_ext_as_string(lexer), lexer->l, startPos));
            } else if (lexer->contents[lexer->i + 1] == '#')
            {
                return lexer_advance_with_token(lexer, init_token(TOKEN_COMMENT_VALUE_MULTI, lexer_get_operator_ext_as_string(lexer), lexer->l, startPos));
            } else
            {
                return lexer_advance_with_token(lexer, init_token(TOKEN_MULT, lexer_get_current_char_as_string(lexer), lexer->l, startPos));
            }
            break;
        case '%':
            if (lexer->contents[lexer->i + 1] == '=') 
            {
                return lexer_advance_with_token(lexer, init_token(TOKEN_MOD_ASGN, lexer_get_operator_ext_as_string(lexer), lexer->l, startPos));
            } else
            {
                return lexer_advance_with_token(lexer, init_token(TOKEN_LESS, lexer_get_current_char_as_string(lexer), lexer->l, startPos));
            }
            break;
        case '^':
            return lexer_advance_with_token(lexer, init_token(TOKEN_EXP, lexer_get_current_char_as_string(lexer), lexer->l, startPos));
            break;
        case '|':
            if (lexer->contents[lexer-> i + 1] == '|')
            {
                return lexer_advance_with_token(lexer, init_token(TOKEN_OR, lexer_get_operator_ext_as_string(lexer), lexer->l, startPos));
            } else
            {
                return lexer_advance_with_token(lexer, init_token(TOKEN_UNKNOWN, lexer_get_current_char_as_string(lexer), lexer->l, startPos));
            }
            break;
        case '&':
            if (lexer->contents[lexer-> i + 1] == '&')
            {
                return lexer_advance_with_token(lexer, init_token(TOKEN_AND, lexer_get_operator_ext_as_string(lexer), lexer->l, startPos));
            } else
            {
                return lexer_advance_with_token(lexer, init_token(TOKEN_UNKNOWN, lexer_get_current_char_as_string(lexer), lexer->l, startPos));
            }
            break;
        case '!':
            if (lexer->contents[lexer->i + 1] == '=') 
            {
                return lexer_advance_with_token(lexer, init_token(TOKEN_NOT_EQ, lexer_get_operator_ext_as_string(lexer), lexer->l, startPos));
            } else
            {
                return lexer_advance_with_token(lexer, init_token(TOKEN_NEGATE, lexer_get_current_char_as_string(lexer), lexer->l, startPos));
            }
            break;
        case ':':
            return lexer_advance_with_token(lexer, init_token(TOKEN_COLON, lexer_get_current_char_as_string(lexer), lexer->l, startPos));
            break;
        case '_':
            return lexer_advance_with_token(lexer, init_token(TOKEN_UNDERSCORE, lexer_get_current_char_as_string(lexer), lexer->l, startPos));
            break;
        case ',':
            return lexer_advance_with_token(lexer, init_token(TOKEN_COMMA, lexer_get_current_char_as_string(lexer), lexer->l, startPos));
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
            return lexer_advance_with_token(lexer, init_token(TOKEN_UNKNOWN, lexer_get_current_char_as_string(lexer), lexer->l, startPos));
            break;
        }
    }
    return (void *)0;
};


// for string literals
// if the current character is " call this function and advance the lexer
token_t *lexer_collect_string(lexer_t *lexer)
{
    int startPos = lexer->lc;
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

    return init_token(TOKEN_STRING, value, lexer->l, startPos-1);
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
    int startPos = lexer->lc;
    while (isalpha(lexer->c))
    {
        char *s = lexer_get_current_char_as_string(lexer);
        // reallocate the string length of the value by adding the length of s to update it and fit the string.
        value = realloc(value, (strlen(value) + strlen(s) + 1) * sizeof(char));
        strcat(value, s); // append the current character to value string.
        lexer_advance(lexer);
    }


    return init_token(TOKEN_ID, value, lexer->l, startPos);
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
    int startPos = lexer->lc;
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
                                return init_token(TOKEN_WHLE, value, lexer->l, startPos);
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
                                return init_token(TOKEN_ELSE, value, lexer->l, startPos);
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
                                    return init_token(TOKEN_TRY, value, lexer->l, startPos);
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
                        return init_token(TOKEN_FOR, value, lexer->l, startPos);
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
                                                    return init_token(TOKEN_FLOAT, value, lexer->l, startPos);
                                                else 
                                                    goto SCAN_INDENTIFIER;
                                            } else goto SCAN_INDENTIFIER;
                                        } else goto SCAN_INDENTIFIER;
                                    } else goto SCAN_INDENTIFIER;
                                } else goto SCAN_INDENTIFIER;
                            } else goto SCAN_INDENTIFIER;
                        } else if (value[5] == '\0') {
                            return init_token(TOKEN_FLOAT, value, lexer->l, startPos);
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
                            return init_token(TOKEN_PRINT, value, lexer->l, startPos);
                    }
                    else if(value[4] == 't') {
                        if(value[5] == '\0') 
                            return init_token(TOKEN_GLOBAL, value, lexer->l, startPos);
                    } else goto SCAN_INDENTIFIER;
                } else goto SCAN_INDENTIFIER;
            } else goto SCAN_INDENTIFIER;
        } else if (value[1] == 'i') {
            if (value[2] == 'n') {
                if (value[3] == 'y') {
                    if (value[4] == 'a') {
                        if(value[5] == '\0') 
                            return init_token(TOKEN_STR, value, lexer->l, startPos);
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
                            return init_token(TOKEN_BOOLT, value, lexer->l, startPos);
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
                            return init_token(TOKEN_BREAK, value, lexer->l, startPos);
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
                            return init_token(TOKEN_CONTINUE, value, lexer->l, startPos);
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
                                return init_token(TOKEN_INT, value, lexer->l, startPos);
                            else 
                                goto SCAN_INDENTIFIER;
                        } else 
                            goto SCAN_INDENTIFIER;
                    } else 
                        goto SCAN_INDENTIFIER; 
                } else if (value[3] == '\0') {
                    return init_token(TOKEN_INT, value, lexer->l, startPos);
                } else 
                    goto SCAN_INDENTIFIER;
            } else 
                goto SCAN_INDENTIFIER;
        } else if (value[1] == 'a') {
            if (value[2] == 's') {
                if (value[3] == 'a') {
                    if (value[4] == '\0') {
                        return init_token(TOKEN_IN, value, lexer->l, startPos);
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
                                        return init_token(TOKEN_CHAR, value, lexer->l, startPos);
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
                    return init_token(TOKEN_CHAR, value, lexer->l, startPos);
                } else goto SCAN_INDENTIFIER;
            } else goto SCAN_INDENTIFIER;
        } else if (value[1] == 'u') {
            if (value[2] == 'n') {
                if (value[3] == 'd') {
                    if (value[4] == 'i') {
                        if (value[5] == '\0')
                            return init_token(TOKEN_ELIF, value, lexer->l, startPos);
                        else 
                            goto SCAN_INDENTIFIER;
                    } else 
                        goto SCAN_INDENTIFIER;
                }
                else if (value[3] == 'g') {
                    if (value[4] == '\0')
                        return init_token(TOKEN_IF, value, lexer->l, startPos);
                    else
                        goto SCAN_INDENTIFIER;
                } else goto SCAN_INDENTIFIER; 
            } else if (value[2] == 'h') {
                if (value[3] == 'a') {
                    if (value[4] == '\0')
                        return init_token(TOKEN_SCAN, value, lexer->l, startPos);
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
                                                return init_token(TOKEN_FUNCTION, value, lexer->l, startPos);
                                            else
                                                goto SCAN_INDENTIFIER;
                                        } else goto SCAN_INDENTIFIER;
                                    } else goto SCAN_INDENTIFIER;
                                } else goto SCAN_INDENTIFIER;
                            } else goto SCAN_INDENTIFIER;
                        } else goto SCAN_INDENTIFIER;
                    } else goto SCAN_INDENTIFIER; 
                } else if(value[3] == '\0'){
                    return init_token(TOKEN_FUNCTION, value, lexer->l, startPos);
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
                                                    return init_token(TOKEN_DBL, value, lexer->l, startPos);
                                                } else
                                                    goto SCAN_INDENTIFIER;
                                            } else goto SCAN_INDENTIFIER;
                                        } else goto SCAN_INDENTIFIER;
                                    } else goto SCAN_INDENTIFIER;
                                } else goto SCAN_INDENTIFIER;
                            } else goto SCAN_INDENTIFIER;
                        } else if (value[5] == '\0') 
                            return init_token(TOKEN_DBL, value, lexer->l, startPos);
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
                                return init_token(TOKEN_IMPORT, value, lexer->l, startPos);
                            else
                                goto SCAN_INDENTIFIER;
                        } else goto SCAN_INDENTIFIER;
                    } else goto SCAN_INDENTIFIER;
                } else goto SCAN_INDENTIFIER;
            } else goto SCAN_INDENTIFIER;
        } else if (value[1] == 't') {
            if (value[2] == '\0')
                return init_token(TOKEN_AND, value, lexer->l, startPos);
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
                                    return init_token(TOKEN_EXCEPT, value, lexer->l, startPos);
                                } else goto SCAN_INDENTIFIER;
                            } else goto SCAN_INDENTIFIER;
                        } else goto SCAN_INDENTIFIER;
                    } else if (value[4] == '\0') {
                        return init_token(TOKEN_BOOLM, value, lexer->l, startPos);
                    } else 
                        goto SCAN_INDENTIFIER;
                } else goto SCAN_INDENTIFIER;
            } else goto SCAN_INDENTIFIER; 
        } else if (value[1] == 'u') {
            if (value[2] == 'l') {
                if (value[3] == 'a') {
                    if (value[4] == '\0') {
                        return init_token(TOKEN_FROM, value, lexer->l, startPos);
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
                            return init_token(TOKEN_FINALLY, value, lexer->l, startPos);
                        } else
                            goto SCAN_INDENTIFIER;
                    } else goto SCAN_INDENTIFIER;
                } else goto SCAN_INDENTIFIER;
            } else if (value[2] == 'l') {
                if (value[3] == 'a') {
                    if (value[4] == '\0')
                        return init_token(TOKEN_NULL, value, lexer->l, startPos);
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
                            return init_token(TOKEN_PRINT, value, lexer->l, startPos);
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
                            return init_token(TOKEN_BREAK, value, lexer->l, startPos);
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
                            return init_token(TOKEN_CONTINUE, value, lexer->l, startPos);
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
                            return init_token(TOKEN_RETURN, value, lexer->l, startPos);
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
                                return init_token(TOKEN_BOOL, value, lexer->l, startPos);
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
            return init_token(TOKEN_OR, value, lexer->l, startPos);
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
                            return init_token(TOKEN_DEL, value, lexer->l, startPos);
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
            return init_token(TOKEN_CAPITAL, value, lexer->l, startPos);
        }
        if (flag_illegal_symbol == 0)   
            return init_token(TOKEN_ID, value, lexer->l, startPos);
        else
            return init_token(TOKEN_UNKNOWN, value, lexer->l, startPos);
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
    int startPos = lexer->lc;
    // while((isdigit(lexer->c) == 1)) -> nagiinfinite loop ako here
    // so pinalitan ko lang ng ganito
    while ((isdigit(lexer->c) || lexer->c == '.'))
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
        return init_token(TOKEN_NUM, value, lexer->l, startPos);
    }
    else if (decimal_count <= 1)
    {
        return init_token(TOKEN_FLOAT_LIT, value, lexer->l, startPos);
    }
    else
    {
        return init_token(TOKEN_UNKNOWN, value, lexer->l, startPos);
    }
}

token_t *lexer_collect_comment_single(lexer_t *lexer)
{
    int startPos = lexer->lc;
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
    return init_token(TOKEN_COMMENT_VALUE_SINGLE, value, lexer->l, startPos-1);
};

token_t *lexer_collect_comment_multi(lexer_t *lexer)
{
    int startPos = lexer->lc;
    int startLinePos = lexer->l;
    lexer_advance(lexer);
    lexer_advance(lexer);
    char *value = calloc(1, sizeof(char));
    value[0] = '\0';
    while (!(lexer->c == '*' && lexer->contents[lexer->i + 1] == '#') && lexer->c != '\0')
    {
        char *s = lexer_get_current_char_as_string(lexer);
        value = realloc(value, (strlen(value) + strlen(s) + 1) * sizeof(char));
        strcat(value, s);
        if (lexer->c == '\n') {
            lexer->l += 1;
        }
        lexer_advance(lexer);
    }
    if (lexer->c == '*' && lexer->contents[lexer->i+1] == '#')
    {
        lexer_advance(lexer); //for *
        lexer_advance(lexer); //for #
    } else 
    {
        return init_token(TOKEN_UNKNOWN, value, startLinePos, startPos-1);
    }
    
    return init_token(TOKEN_COMMENT_VALUE_MULTI, value, startLinePos, startPos-1);
};

token_t *lexer_collect_char_lit(lexer_t *lexer)
{
    int startPos = lexer->lc;
    lexer_advance(lexer);
    char *value = calloc(2, sizeof(char));
    value[0] = lexer->c;
    value[1] = '\0';
    if (lexer->contents[lexer->i + 1] == 39)
    {
        lexer_advance(lexer);
        lexer_advance(lexer);
        return init_token(TOKEN_CHAR_LIT, value, lexer->l, startPos);
    } else
    {
        lexer_advance(lexer);
        lexer_advance(lexer);
        return init_token(TOKEN_UNKNOWN, value, lexer->l, startPos);
    }
};