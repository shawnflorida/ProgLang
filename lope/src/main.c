#include <stdio.h>
#include <stdlib.h>
#include "include/main.h"
int main(int argc, char **argv)
{

    fptr = fopen(argv[1], "r");
    if (fptr != NULL)
    {
        readFile();
        lexer = init_lexer(fileContent);
        scanFile(lexer);
    }
    else
    {
        lexer = init_lexer(
            "linya pangalan = \"shawn michael florida\";"
            "lahad(pangalan);"
            "numero number = 1;");

        // numero a = 1;
        scanFile(lexer);
    }
    return 0;
}

int scanFile(lexer_t *lexer)
{

    token_t *token = (void *)0;

    while ((token = lexer_get_next_token(lexer)) != (void *)0)
    {
        printf("TOKEN(%d,%s)\n", token->type, token->value);
    }
}

int readFile()
{
    fseek(fptr, 0L, SEEK_END);
    fileLen = ftell(fptr);
    fseek(fptr, 0L, SEEK_SET);

    fileContent = calloc(fileLen, sizeof(char));
    fread(fileContent, sizeof(char), fileLen, fptr);
}