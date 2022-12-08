#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "include/fileHandler.h"

int *find_file()
{
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
        fprintf(output_ptr, "TOKEN(%s,%s)\n", token_type[token->type], token->value);
    }
}