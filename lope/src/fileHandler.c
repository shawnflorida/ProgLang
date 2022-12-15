#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "include/fileHandler.h"
#include "include/token.h"

int *find_file()
{
    //file -> fseek -> from start to finish -> amount of character length to memory -> 
    fseek(input_ptr, 0L, SEEK_END);
    fileLen = ftell(input_ptr);
    fseek(input_ptr, 0L, SEEK_SET);

    fileContent = calloc(fileLen, sizeof(char));
    fread(fileContent, sizeof(char), fileLen, input_ptr);
}
int check_Extension(char *file)
{
    char *extension;
    extension = strrchr(file, '.');
    if (compare_to_keyword(extension, ".lp") == 1)
    {

        return 1;
    }
    return 0;
}
void create_TokenOutput(char *file)
{
    memset(fileName, 0, sizeof(fileName));
    strncpy(fileName, file, strrchr(file, '.') - file);
    strcat(fileName, "_SymbolTable");
    strcat(fileName, ".txt");
    output_ptr = fopen(fileName, "w");
}

int scanFile(lexer_t *lexer)
{

    token_t *token = (void *)0;
    const char *token_type[] = {
        "Identifier/Value",
        "Equal Sign",
        "Semi-Colon",
        "Left Parenthesis",
        "Right Parenthesis",
        "Left Bracket",
        "Right Bracket",
        "Left Brace",
        "Right Brace",
        "Comment",
        "Addition",
        "Subtraction",
        "Division",
        "Integer Division",
        "Multiplication",
        "Modulus",
        "Exponent",
        "Lesser Than",
        "Greater Than",
        "Not",
        "Colon",
        "Underscore",
        "Increment",
        "Decrement",
        "Equal To",
        "Less than or Equal to",
        "Greater than or Equal to",
        "Not Equal to",
        "Addition Assignment",
        "Subtraction Assignment",
        "Multiplication Assignment",
        "Division Assignment",
        "Modulo Assignment",
        "Integer Division Assignment",
        "While",
        "If",
        "Else",
        "Else If",
        "For",
        "Function",
        "Import",
        "Try",
        "Except",
        "Finally",
        "Break",
        "Continue",
        "Return",
        "And",
        "Or",
        "In",
        "Global",
        "Boolean",
        "True",
        "False",
        "Integer",
        "Num Literal",
        "String",
        "String Literal",
        "None",
        "Float",
        "Double",
        "Character",
        "Invalid",
        "Single Comment",
        "Multiple Comment",
        "Constant",
        "Comma Separator",
        "Char literal"
        };

    const char *token_code[] = {
        "TOKEN_ID",
        "TOKEN_EQUALS",
        "TOKEN_SEMI",
        "TOKEN_LPAREN",
        "TOKEN_RPAREN",
        "TOKEN_LBRACKET",
        "TOKEN_RBRACKET",
        "TOKEN_LBRACE",
        "TOKEN_RBRACE",
        "TOKEN_COMMENT",
        "TOKEN_ADD",
        "TOKEN_SUB",
        "TOKEN_DIV",
        "TOKEN_INTDIV",
        "TOKEN_MULT",
        "TOKEN_MOD",
        "TOKEN_EXP",
        "TOKEN_LESS",
        "TOKEN_GREATER",
        "TOKEN_NEGATE",
        "TOKEN_COLON",
        "TOKEN_UNDERSCORE",
        "TOKEN_INCR",
        "TOKEN_DECR",
        "TOKEN_EQ_TO",
        "TOKEN_LS_THAN_EQ",
        "TOKEN_GR_THAN_EQ",
        "TOKEN_NOT_EQ",
        "TOKEN_ADD_ASGN",
        "TOKEN_SUB_ASGN",
        "TOKEN_MULT_ASGN",
        "TOKEN_DIV_ASGN",
        "TOKEN_MOD_ASGN",
        "TOKEN_INTDIV_ASGN",
        "TOKEN_WHLE",
        "TOKEN_IF",
        "TOKEN_ELSE",
        "TOKEN_ELIF If",
        "TOKEN_FOR",
        "TOKEN_FUNCTION",
        "TOKEN_IMPORT",
        "TOKEN_TRY",
        "TOKEN_EXCEPT",
        "TOKEN_FINALLY",
        "TOKEN_BREAK",
        "TOKEN_CONTINUE",
        "TOKEN_RETURN",
        "TOKEN_AND",
        "TOKEN_OR",
        "TOKEN_IN",
        "TOKEN_GLOBAL",
        "TOKEN_BOOL",
        "TOKEN_BOOLT",
        "TOKEN_BOOLM",
        "TOKEN_INT",
        "TOKEN_NUM",
        "TOKEN_STR",
        "TOKEN_STRING",
        "TOKEN_NULL",
        "TOKEN_FLOAT",
        "TOKEN_DBL",
        "TOKEN_CHAR",
        "TOKEN_UNKNOWN",
        "TOKEN_COMMENT_VALUE_SINGLE",
        "TOKEN_COMMENT_VALUE_MULTI",
        "TOKEN_CAPITAL",
        "TOKEN_COMMA",
        "TOKEN_CHAR_LIT"
        };

    printf("TOKEN-NUMBER\tTOKEN-CODE\t\t\tTOKEN-INFO\t\tLEXEME\n");
    fprintf(output_ptr, "TOKEN-NUMBER\tTOKEN-CODE\t\t\tTOKEN-INFO\t\tLEXEME\n");
    printf("---------------------------------------------------------------------------------------------------------------------\n");
    fprintf(output_ptr, "---------------------------------------------------------------------------------------------------------------------\n");
    int i = 1;
    while ((token = lexer_get_next_token(lexer)) != (void *)0)
    {
        if(token->type == 0 || token->type == 3 || token->type == 4)
        {
            printf("%d\t\t%s\t\t\t%s\t%s\n", i, token_code[token->type], token_type[token->type], token->value);
            fprintf(output_ptr, "%d\t\t%s\t\t\t%s\t%s\n", i, token_code[token->type], token_type[token->type], token->value);
        }
        else if(token->type == 1 || token->type == 2 || token->type == 57 || token->type == 55 || token->type == 65 || token->type == 67 || token->type == 45 || token->type == 7 || token->type == 8 || token->type == 39 || token->type == 22)
        {
            printf("%d\t\t%s\t\t\t%s\t\t%s\n", i, token_code[token->type], token_type[token->type], token->value);
            fprintf(output_ptr, "%d\t\t%s\t\t\t%s\t\t%s\n", i, token_code[token->type], token_type[token->type], token->value);
        }
        else if(token->type == 63 || token->type == 64)
        {
            if(token->type == 63)
            {
                printf("%d\t\t%s\t%s\t\t%s\n", i, token_code[token->type], token_type[token->type], token->value);
                fprintf(output_ptr, "%d\t\t%s\t%s\t\t%s\n", i, token_code[token->type], token_type[token->type], token->value);
            }
            else
            {
                for(int i=0; i<strlen(token->value); i++)
                    if(token->value[i] == '\n')
                        token->value[i] = ' ';
                printf("%d\t\t%s\t%s\t%s\n",  i, token_code[token->type], token_type[token->type], token->value);
                fprintf(output_ptr, "%d\t\t%s\t%s\t%s\n",  i, token_code[token->type], token_type[token->type], token->value);
            }
        }
        else
            if(token->type == 25) 
            {
                printf("%d\t\t%s\t\t%s\t%s\n", i, token_code[token->type], token_type[token->type], token->value);
                fprintf(output_ptr, "%d\t\t%s\t\t%s\t%s\n", i, token_code[token->type], token_type[token->type], token->value);
            }
            else
            {
                printf("%d\t\t%s\t\t\t%s\t\t\t%s\n", i, token_code[token->type], token_type[token->type], token->value);
                fprintf(output_ptr, "%d\t\t%s\t\t\t%s\t\t\t%s\n", i, token_code[token->type], token_type[token->type], token->value);
            }
        printf("---------------------------------------------------------------------------------------------------------------------\n");
        fprintf(output_ptr, "---------------------------------------------------------------------------------------------------------------------\n");
        i++;
    }
}