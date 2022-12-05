#include <stdio.h>
#include <stdlib.h>
#include "include/main.h"
#include "include/addFile.h"
#include "include/lexer.h"

int main(int argc, char **argv)
{

    for (int i = 1; i < argc; i++)
    {
        fptr = fopen(argv[i], "r");
        if (fptr != NULL)
        {
            find_file();
            lexer = init_lexer(fileContent);
            scanFile(lexer);
        }
        else
        {
            lexer = init_lexer(
                "linya error = \"may problema\";");

            // numero a = 1;
            scanFile(lexer);
        }
    }
    return 0;

}



int scanFile(lexer_t *lexer)
{

    token_t *token = (void *)0;
    const char *token_type[] = {
        "Identifier/Value",
        "Equal Sign",
        "String",
        "Semi-Colon",
        "Left Parenthesis",
        "Right Parenthesis",
        "Addition",
        "Subtraction",
        "Multiplication",
        "Division",
        "Exponent"};

    while ((token = lexer_get_next_token(lexer)) != (void *)0)
    {
        printf("TOKEN(%s,%s)\n", token_type[token->type], token->value);
    }
}
