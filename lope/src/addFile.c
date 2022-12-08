#include <stdlib.h>
#include <stdio.h>
#include "include/addFile.h"
#include "include/main.h"

int *find_file()
{
    fseek(fptr, 0L, SEEK_END);
    fileLen = ftell(fptr);
    fseek(fptr, 0L, SEEK_SET);

    fileContent = calloc(fileLen, sizeof(char));
    fread(fileContent, sizeof(char), fileLen, fptr);
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
        "Multiplication",
        "Modulus",
        "Exponent",
        "Lesser Than",
        "Greater Than",
        "Not",
        "Colon",
        "Underscore",
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
        "Multiple Comment"
        };

    while ((token = lexer_get_next_token(lexer)) != (void *)0)
    {
        printf("TOKEN(%s,%s)\n", token_type[token->type], token->value);
    }
}